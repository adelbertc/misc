#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using std::atof;
using std::cerr;
using std::cout;
using std::endl;
using std::find_if;
using std::ifstream;
using std::isspace;
using std::make_pair;
using std::ofstream;
using std::pair;
using std::sort;
using std::string;
using std::vector;

// Adelbert Chang adelbertc@gmail.com

vector< pair<double, double> > extract_cdf(vector<double> dataVector);
double get_value_at(double percentage, vector< pair<double, double> > cdf);
bool space(char c);
bool not_space(char c);
double last_in_line(const string& str);
vector<double> process_cdf_file(string filename);
string remove_last_extension(string filename);
void write_cdf(vector< pair<double, double> > cdf, string outputFilename);
void write_cdf_info(vector< pair<double, double> > cdf, string outputFilename);

int main (int argc, char **argv)
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input filename>" << endl;
        return 1;
    }
    cerr << "Processing command line args..." << endl;
    string filename = argv[1];
    string fileprefix = remove_last_extension(filename);

    cerr << "Processing input file..." << endl;
    vector<double> dataVector = process_cdf_file(filename);

    cerr << "Extracting cdf..." << endl;
    vector< pair<double, double> > cdf = extract_cdf(dataVector);

    cerr << "Writing cdf..." << endl;
    write_cdf(cdf, fileprefix + ".cdf");
    write_cdf_info(cdf, fileprefix + ".cdfInfo");

    cerr << "Done!" << endl;
    return 0;
}

vector< pair<double, double> > extract_cdf(vector<double> dataVector)
{
    /*
     Much thanks to this guy: http://www.srcco.de/v/cdf-plots-perf-eval
    */
    vector<double>::size_type amountOfData = dataVector.size();
    vector< pair<double, double> > cdf(amountOfData);
    
    double prob = 0.0;
    vector< pair<double, double> >::size_type position = 0;
    for (vector<double>::const_iterator iter = dataVector.begin();
         iter != dataVector.end(); ++iter, ++position) {
        prob += 1.0 / amountOfData;
        cdf[position] = make_pair(prob, *iter);
    }
    return cdf;
}

double get_value_at(double percentage, vector< pair<double, double> > cdf)
{
    if (percentage > 1)
        percentage /= 100.0;
    
    double prev;
    for(vector< pair<double, double> >::const_iterator iter = cdf.begin();
        iter != cdf.end(); ++iter) {
        double prob = iter->first;
        double data = iter->second;
        // Only reliable if there is LOTS of data
        if (prob > percentage)
            return prev;
        prev = data;
    }
}

bool space(char c)
{
    return isspace(c);
}

bool not_space(char c)
{
    return !isspace(c);
}

double last_in_line(const string& str)
{
    typedef string::const_iterator iter;
    vector<string> ret;
    
    iter i = str.begin();
    while (i != str.end()) {
        i = find_if(i, str.end(), not_space);
        iter j = find_if(i, str.end(), space);
        
        if (i != str.end())
            ret.push_back(string(i, j));
        i = j;
    }
    return atof(ret[ret.size() - 1].c_str());
}

vector<double> process_cdf_file(string filename)
{
    ifstream infile(filename.c_str());
    vector<double> dataVector;
    string line;
    while (getline(infile, line)) {
        // Assume relevant data is end of line
        dataVector.push_back(last_in_line(line));
    }
    infile.close();
    sort(dataVector.begin(), dataVector.end());
    return dataVector;
}

string remove_last_extension(string filename)
{
    int i = filename.size() - 1;
    for( ; i >= 0; --i) {
        if (filename[i] == '.')
            break;
    }
    if (i > 0) {
        string temp(filename, 0, i);
        return temp;
    }
    return filename;
}

void write_cdf(vector< pair<double, double> > cdf, string outputFilename)
{
    ofstream outfile(outputFilename.c_str());
    for (vector< pair<double, double> >::const_iterator iter = cdf.begin();
         iter != cdf.end(); ++iter) {
        outfile << iter->first << " " << iter->second << endl;
    }
    outfile.close();
}

void write_cdf_info(vector< pair<double, double> > cdf, string outputFilename)
{
    ofstream outfile(outputFilename.c_str());
    for (double i = 0.1; i <= 1; i += 0.1)
        outfile << i << " " << get_value_at(i, cdf) << endl;
    outfile.close();
}