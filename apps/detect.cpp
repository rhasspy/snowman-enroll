#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <stdexcept>

#include <helper.h>
#include <snowboy-detect.h>

using namespace std;

struct RunConfig {
  filesystem::path modelPath;
  optional<filesystem::path> resourcesPath;
  std::string sensitivity = "0.5";
  float audioGain = 1.0;
  bool applyFrontend = false;
  size_t samplesPerChunk = 512;
};

void parseArgs(int argc, char *argv[], RunConfig &runConfig);

int main(int argc, char **argv) try {
  RunConfig runConfig;
  parseArgs(argc, argv, runConfig);

  // Re-open stdin in binary mode
  freopen(NULL, "rb", stdin);

  if (!runConfig.resourcesPath) {
    runConfig.resourcesPath = filesystem::path("resources/common.res");
  }

  snowboy::SnowboyDetect detector(runConfig.resourcesPath->c_str(),
                                  runConfig.modelPath.c_str());
  detector.SetSensitivity(runConfig.sensitivity);
  detector.SetAudioGain(runConfig.audioGain);
  detector.ApplyFrontend(runConfig.applyFrontend);

  int16_t samples[runConfig.samplesPerChunk];

  while (true) {
    fread(samples, sizeof(int16_t), runConfig.samplesPerChunk, stdin);
    auto s = detector.RunDetection(samples, runConfig.samplesPerChunk, false);
    if (s > 0) {
      std::cout << s << std::endl;
    }
  }
  return 0;
} catch (const std::exception &e) {
  std::cerr << "Error: " << e.what() << std::endl;
  return -1;
}

void printUsage(char *argv[]) {
  cerr << endl;
  cerr << "usage: " << argv[0] << " [options]" << endl;
  cerr << endl;
  cerr << "options:" << endl;
  cerr << "   -h        --help                show this message and exit"
       << endl;
  cerr << "   -m  FILE  --model        FILE   path to wake word model file "
          "(required)"
       << endl;
  cerr << "   -s  VALUE --sensitivity  VALUE  wake word sensitivity (default: "
          "0.5)"
       << endl;
  cerr << "   --audio-gain             NUM    audio gain (default: 1.0)"
       << endl;
  cerr << "   --apply-frontend                apply audio frontend" << endl;
  cerr << "   --resources              FILE   path to common.res" << endl;
  cerr << "   --samples-per-chunk      NUM    number of samples to read from "
          "stdin at a time (default: 512)"
       << endl;
  cerr << endl;
}

void ensureArg(int argc, char *argv[], int argi) {
  if ((argi + 1) >= argc) {
    printUsage(argv);
    exit(0);
  }
}

// Parse command-line arguments
void parseArgs(int argc, char *argv[], RunConfig &runConfig) {
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];

    if (arg == "-m" || arg == "--model") {
      ensureArg(argc, argv, i);
      runConfig.modelPath = filesystem::path(argv[++i]);
    } else if (arg == "-s" || arg == "--sensitivity") {
      ensureArg(argc, argv, i);
      runConfig.sensitivity = std::string(argv[++i]);
    } else if (arg == "--audio-gain" || arg == "--audio_gain") {
      ensureArg(argc, argv, i);
      runConfig.audioGain = stof(argv[++i]);
    } else if (arg == "--apply-frontend" || arg == "--apply_frontend") {
      ensureArg(argc, argv, i);
      runConfig.applyFrontend = true;
    } else if (arg == "--samples-per-chunk" || arg == "--samples_per_chunk") {
      ensureArg(argc, argv, i);
      runConfig.samplesPerChunk = stoi(argv[++i]);
    } else if (arg == "--resources") {
      ensureArg(argc, argv, i);
      runConfig.resourcesPath = filesystem::path(argv[++i]);
    } else if (arg == "-h" || arg == "--help") {
      printUsage(argv);
      exit(0);
    }
  }

  // Verify model file exists
  ifstream modelFile(runConfig.modelPath.c_str(), ios::binary);
  if (!modelFile.good()) {
    throw runtime_error("Model file doesn't exist");
  }
}
