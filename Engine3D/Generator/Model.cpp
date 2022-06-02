#include "Model.h"

using namespace std;

int generateModelFile(string filename, vector<float>& vertexB, vector<unsigned int>& indexB, vector<float>& normalB, vector<float>& textB) {
	ofstream fich;
	fich.open(filename, ios::out);

    if(fich.is_open()) {
        //Writes contents of vertexB
        for (int i = 0; i < vertexB.size() - 1; i++) {
            fich << vertexB[i] << ";";
        }
        fich << vertexB[vertexB.size() - 1] << "\n";

        //Writes contents of indexB
        for (int i = 0; i < indexB.size() - 1; i++) {
            fich << indexB[i] << ";";
        }

        fich << indexB[indexB.size() - 1] << "\n";

        //Writes contents of normalB
        for (int i = 0; i < normalB.size() - 1; i++) {
            fich << normalB[i] << ";";
        }

        fich << normalB[normalB.size() - 1] << "\n";

        //Writes contents of textB
        for (int i = 0; i < textB.size() - 1; i++) {
            fich << textB[i] << ";";
        }

        fich << textB[textB.size() - 1] << "\n";

        fich.close();
        return 0;
    }

    return -1;
}

int readModelFromFile(string filename, vector<float>& vertexB, vector<unsigned int>& indexB, vector<float>& normalB, vector<float>& textB,float *cube) {
    std::ifstream fd;
    fd.open(filename, ios::in);

    int index = 0;
    float value = 0;

    if (fd.fail()) return -1;
	else {
		string line;
		string delimiter = ";";

		//Read vertices
		getline(fd, line);
		vector<string> vB = parseLine(line, delimiter);
        vertexB.reserve(vB.size());
        for(int i = 0; i < vB.size() ; i++){
            value = stof(vB[i]);
            cube[index * 2] = (cube[index * 2] > value)?cube[index * 2]:value;
            cube[index * 2 + 1] = (cube[index * 2 + 1] < value)?cube[index * 2 + 1]:value;
            vertexB.push_back(value);
            index = (index + 1) % 3;
        }

        //Read indexes
        getline(fd, line);
		vector<string> iB = parseLine(line, delimiter);
        indexB.reserve(iB.size());
        for(int i = 0; i < iB.size() ; i++)
            indexB.push_back(stoul(iB[i]));

        //Read normal
        getline(fd, line);
        vector<string> nB = parseLine(line, delimiter);
        normalB.reserve(nB.size());
        for(int i = 0; i < nB.size() ; i++)
            normalB.push_back(stof(nB[i]));

        //Read text
        getline(fd, line);
        vector<string> tB = parseLine(line, delimiter);
        textB.reserve(nB.size());
        for(int i = 0; i < tB.size() ; i++)
            textB.push_back(stof(tB[i]));

        return 0;
	}
}