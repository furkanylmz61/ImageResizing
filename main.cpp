#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <Magick++.h>
#include <sys/stat.h>
#include <sys/types.h>

void resizeImage(const std::string& inputPath, const std::string& outputPath, int width, int height) {
    Magick::Image image;
    try {
        image.read(inputPath);
        image.resize(Magick::Geometry(width, height));
        image.write(outputPath);
        std::cout << "Resized " << inputPath << " and saved to " << outputPath << std::endl;
    } catch (Magick::Exception &error) {
        std::cerr << "Error resizing image: " << error.what() << std::endl;
    }
}

bool createDirectory(const std::string& path) {
    struct stat info;

    if (stat(path.c_str(), &info) != 0) {
        // Dizin yok, oluşturmaya çalışalım
        if (mkdir(path.c_str(), 0777) != 0) {
            std::cerr << "Error creating directory: " << path << std::endl;
            return false;
        }
    } else if (!(info.st_mode & S_IFDIR)) {
        // Path var ama bir dizin değil
        std::cerr << "Path exists but is not a directory: " << path << std::endl;
        return false;
    }
    return true;
}

int main() {
    // Örnek resim dosyaları ve çıktı dosyası yolları
    std::vector<std::string> inputFiles = {"input1.jpg", "input2.jpg", "input3.jpg"};
    std::string outputDir = "output/";

    // Çıktı dizininin var olduğundan emin ol
    if (!createDirectory(outputDir)) {
        return 1; // Dizin oluşturulamazsa programdan çık
    }

    int width = 800; // Yeni genişlik
    int height = 600; // Yeni yükseklik

    // Çıktı dosyaları için vektör
    std::vector<std::string> outputFiles;
    for (const auto& inputFile : inputFiles) {
        std::string outputFile = outputDir + "resized_" + inputFile;
        outputFiles.push_back(outputFile);
    }

    // Resim boyutlandırma işlemleri için thread'ler oluştur
    std::vector<std::thread> threads;
    for (size_t i = 0; i < inputFiles.size(); ++i) {
        threads.emplace_back(resizeImage, inputFiles[i], outputFiles[i], width, height);
    }

    // Tüm thread'lerin tamamlanmasını bekle
    for (auto& thread : threads) {
        thread.join();
    }

    // Çıktı dosyalarını yazdır
    for (const auto& outputFile : outputFiles) {
        std::cout << "Output file: " << outputFile << std::endl;
    }

    return 0;
}
