#include <GL/glut.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <fftw3.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <memory>

// === Constantes de Configuração === //
constexpr int SAMPLE_RATE = 44100;
constexpr int CHANNELS = 2;
constexpr int FFT_SIZE = 512;
constexpr int BANDS = 64;
constexpr float DB_RANGE = 40.0f;
constexpr float SPECTRUM_MULTIPLIER = 1e-2f;

// === Utilitários === //
class MathUtils
{
public:
  static float clamp(float val, float minv, float maxv)
  {
    return std::max(minv, std::min(maxv, val));
  }
};

// === Classe para Análise Espectral === //
class SpectrumAnalyzer
{
public:
  SpectrumAnalyzer()
      : window(FFT_SIZE), logscale(BANDS + 1), spectrum(BANDS), maxspectrum(BANDS, 0.0f)
  {
    fftIn = (double *)fftw_malloc(sizeof(double) * FFT_SIZE);
    fftOut = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * FFT_SIZE);
    fftPlan = fftw_plan_dft_r2c_1d(FFT_SIZE, fftIn, fftOut, FFTW_ESTIMATE);
    initWindowAndScale();
  }

  ~SpectrumAnalyzer()
  {
    fftw_destroy_plan(fftPlan);
    fftw_free(fftIn);
    fftw_free(fftOut);
  }

  void processAudio(const Sint16 *samples)
  {
    for (int i = 0; i < FFT_SIZE; ++i)
    {
      float l = samples[2 * i] / 32768.0f;
      float r = samples[2 * i + 1] / 32768.0f;
      float mono = (l + r) * 0.5f;
      fftIn[i] = mono * window[i];
    }
    fftw_execute(fftPlan);

    std::vector<float> magnitudes(FFT_SIZE / 2);
    for (int i = 0; i < FFT_SIZE / 2; ++i)
    {
      float re = fftOut[i][0];
      float im = fftOut[i][1];
      float amplitude = SPECTRUM_MULTIPLIER * sqrtf(re * re + im * im);
      magnitudes[i] = MathUtils::clamp(amplitude, 0.0f, 1.0f);
    }

    for (int i = 0; i < BANDS; ++i)
    {
      int a = ceilf(logscale[i]);
      int b = floorf(logscale[i + 1]);
      float sum = 0;

      if (b < a)
      {
        sum += magnitudes[b] * (logscale[i + 1] - logscale[i]);
      }
      else
      {
        if (a > 0)
          sum += magnitudes[a - 1] * (a - logscale[i]);
        for (int j = a; j < b; ++j)
          sum += magnitudes[j];
        if (b < FFT_SIZE / 2)
          sum += magnitudes[b] * (logscale[i + 1] - b);
      }

      sum *= FFT_SIZE / 24.0f;
      float dB = 20.0f * log10f(sum + 1e-6f);
      float val = 1.0f + dB / DB_RANGE;
      spectrum[i] = MathUtils::clamp(val, 0.0f, 1.0f);
    }

    normalizeSpectrum();
  }

  void updateMaxSpectrum(float decay = 0.02f)
  {
    for (int i = 0; i < BANDS; ++i)
    {
      maxspectrum[i] = std::max(spectrum[i], maxspectrum[i]) - decay;
      if (maxspectrum[i] < 0.0f)
        maxspectrum[i] = 0.0f;
    }
  }

  const std::vector<float> &getMaxSpectrum() const { return maxspectrum; }

private:
  double *fftIn;
  fftw_complex *fftOut;
  fftw_plan fftPlan;
  std::vector<float> window;
  std::vector<float> logscale;
  std::vector<float> spectrum;
  std::vector<float> maxspectrum;

  void initWindowAndScale()
  {
    for (int i = 0; i < FFT_SIZE; ++i)
      window[i] = 0.5f * (1 - cos(2 * M_PI * i / (FFT_SIZE - 1)));
    float minLog = log10f(1.0f);
    float maxLog = log10f(FFT_SIZE / 2.0f);
    for (int i = 0; i <= BANDS; ++i)
    {
      float logIndex = pow(10.0f, minLog + (maxLog - minLog) * i / BANDS) - 1.0f;
      logscale[i] = MathUtils::clamp(logIndex, 0.0f, FFT_SIZE / 2.0f - 1.0f);
    }
  }

  void normalizeSpectrum()
  {
    float minVal = *std::min_element(spectrum.begin(), spectrum.end());
    if (minVal < 0.0f)
      for (auto &s : spectrum)
        s -= minVal;
    float maxVal = *std::max_element(spectrum.begin(), spectrum.end());
    if (maxVal > 0.0f)
      for (auto &s : spectrum)
        s /= maxVal;
  }
};

// === Classe para Visualização Circular === //
class CircularSpectrumRenderer
{
public:
  CircularSpectrumRenderer(const SpectrumAnalyzer &analyzer)
      : analyzer(analyzer) {}

  void render() const
  {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    float cx = 0.0f, cy = 0.0f;
    float radius = 0.5f;
    float maxBarLen = 0.4f;

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);

    const auto &maxSpec = analyzer.getMaxSpectrum();
    for (int i = 0; i < BANDS; ++i)
    {
      float angle = 2.0f * M_PI * i / BANDS;
      float spec = MathUtils::clamp(maxSpec[i], 0.0f, 1.0f);
      float barLen = radius + spec * maxBarLen;

      float r = 0.5f + 0.5f * sin(angle);
      float g = 0.5f + 0.5f * sin(angle + 2.0f * M_PI / 3.0f);
      float b = 0.5f + 0.5f * sin(angle + 4.0f * M_PI / 3.0f);
      glColor3f(r, g, b);

      float x0 = cx + radius * cos(angle);
      float y0 = cy + radius * sin(angle);
      float x1 = cx + barLen * cos(angle);
      float y1 = cy + barLen * sin(angle);

      glBegin(GL_LINES);
      glVertex2f(x0, y0);
      glVertex2f(x1, y1);
      glEnd();
    }
    glPopMatrix();
    glutSwapBuffers();
  }

private:
  const SpectrumAnalyzer &analyzer;
};

// === Classe para Gerenciamento de Áudio === //
class AudioManager
{
public:
  AudioManager(SpectrumAnalyzer &analyzer)
      : analyzer(analyzer) {}

  bool initialize()
  {
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
      std::cerr << "Erro SDL: " << SDL_GetError() << std::endl;
      return false;
    }
    if (Mix_OpenAudio(SAMPLE_RATE, MIX_DEFAULT_FORMAT, CHANNELS, 4096) < 0)
    {
      std::cerr << "Erro Mix_OpenAudio: " << Mix_GetError() << std::endl;
      SDL_Quit();
      return false;
    }
    Mix_SetPostMix(AudioManager::audioCallback, this);
    return true;
  }

  bool loadAndPlay(const char *filename)
  {
    music = Mix_LoadMUS(filename);
    if (!music)
    {
      std::cerr << "Erro Mix_LoadMUS: " << Mix_GetError() << std::endl;
      return false;
    }
    Mix_PlayMusic(music, -1);
    return true;
  }

  ~AudioManager()
  {
    if (music)
      Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();
  }

  static void audioCallback(void *udata, Uint8 *stream, int len)
  {
    auto *self = static_cast<AudioManager *>(udata);
    if (!stream || len < FFT_SIZE * CHANNELS * (int)sizeof(Sint16))
      return;
    self->analyzer.processAudio(reinterpret_cast<Sint16 *>(stream));
    glutPostRedisplay();
  }

private:
  SpectrumAnalyzer &analyzer;
  Mix_Music *music = nullptr;
};

// === Classe Principal da Aplicação === //
class SpectrumApp
{
public:
  SpectrumApp(const char *filename)
      : analyzer(), renderer(analyzer), audio(analyzer), mp3file(filename) {}

  int run(int argc, char **argv)
  {
    if (!audio.initialize())
      return 1;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Visualizador de Espectro Circular");

    initGL();
    glutDisplayFunc(SpectrumApp::displayCallback);
    glutIdleFunc(SpectrumApp::idleCallback);
    glutReshapeFunc(SpectrumApp::reshapeCallback);
    glutKeyboardFunc(SpectrumApp::keyboardCallback);

    instance = this;

    if (!audio.loadAndPlay(mp3file))
      return 1;

    glutMainLoop();
    return 0;
  }

private:
  SpectrumAnalyzer analyzer;
  CircularSpectrumRenderer renderer;
  AudioManager audio;
  const char *mp3file;

  static SpectrumApp *instance;

  static void displayCallback()
  {
    if (instance)
      instance->renderer.render();
  }

  static void idleCallback()
  {
    if (instance)
    {
      instance->analyzer.updateMaxSpectrum();
      glutPostRedisplay();
    }
  }

  static void reshapeCallback(int w, int h)
  {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
  }

  static void keyboardCallback(unsigned char key, int, int)
  {
    if (key == 27)
      exit(0); // ESC
  }

  static void initGL()
  {
    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
  }
};
SpectrumApp *SpectrumApp::instance = nullptr;

// === main === //
int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "Uso: " << argv[0] << " <arquivo.mp3>" << std::endl;
    return 1;
  }
  SpectrumApp app(argv[1]);
  return app.run(argc, argv);
}
