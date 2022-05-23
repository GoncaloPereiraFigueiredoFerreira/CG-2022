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

int readModelFromFile(string filename, vector<float>& vertexB, vector<unsigned int>& indexB, vector<float>& normalB, vector<float>& textB) {
    std::ifstream fd;
    fd.open(filename, ios::in);

    if (fd.fail()) return -1;
	else {
		string line;
		string delimiter = ";";

		//Read vertices
		getline(fd, line);
		vector<string> vB = parseLine(line, delimiter);
        vertexB.reserve(vB.size());
        for(int i = 0; i < vB.size() ; i++)
            vertexB.push_back(stof(vB[i]));

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