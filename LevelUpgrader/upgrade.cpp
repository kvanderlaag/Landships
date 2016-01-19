#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <SDL2/SDL.h>

#include <dirent.h>
#include <errno.h>

int main(int argc, char** argv) {
    std::vector<std::string> levelFiles;

    dirent* de;
    DIR* dp;

    std::string basePath = SDL_GetBasePath();
    dp = opendir(basePath.c_str());
    if (dp) {
        while (true) {
            errno = 0;

            de = readdir(dp);
            if (de == NULL)
                break;
            std::string filename = de->d_name;
            if (filename.at(filename.length() - 1) == 'd' && filename.at(filename.length() - 2) == '.') {
                std::string fullPath = basePath + filename;
                std::ifstream inFile(fullPath, std::ios::binary);
                if (inFile.good() ) {
                    char c;
                    char DE = 0xDE;
                    char AD = 0xAD;
                    char BE = 0xBE;
                    char EF = 0xEF;

                    inFile.get(c);
                    if (c != DE)
                        continue;

                    inFile.get(c);
                    if (c != AD)
                        continue;

                    inFile.get(c);
                    if (c != BE)
                        continue;

                    inFile.get(c);
                    if (c != EF)
                        continue;

                    levelFiles.push_back(filename);
                }

            }

        }
    }

    char tiles[30 * 40];

    for (std::string level : levelFiles) {
        std::cout << "Translating level file " << level << std::endl;
        std::string fullPath = basePath + level;
        std::ifstream inFile(fullPath, std::ios::binary);

        char c;
        inFile.get(c);
        inFile.get(c);
        inFile.get(c);
        inFile.get(c);

        for (int i = 0; i < 30 * 40; ++i) {
            inFile.get(c);
            if (c >= 0x06) {
                tiles[i] = c + 1;
            }  else {
                tiles[i] = c;
            }
        }

        inFile.close();

        std::ofstream outFile(fullPath, std::ios::binary);
        char DE = 0xDE;
        char AD = 0xAD;
        char BE = 0xBE;
        char EF = 0xEF;

        outFile << DE << AD << BE << EF;

        for (int i = 0; i < 30 * 40; ++i) {
            outFile << tiles[i];
        }

        outFile.close();

    }

    return 0;
}
