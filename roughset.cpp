#include <bits/stdc++.h>
using namespace std;

map<int, string> inv_decisions;

int** getInput (int& num_objects, int& num_attributes, int& num_decisions) {
    fstream fin;
    fin.open("des_tbl.txt");
    string word;
    fin >> word;
    num_objects = stoi(word);
    fin >> word;
    num_attributes = stoi(word);
    fin >> word;
    num_decisions = stoi(word);
    int** mat = new int*[num_objects];
    for (int i=0; i<num_objects; i++){
        mat[i] = new int[num_attributes + num_decisions];
    }
    map<string, int> decisions;
    int k=1;
    for (int i=0; i<num_objects; i++){
        for (int j=0; j<num_attributes; j++){
            fin >> word;
            if (word == "Y") mat[i][j] = 1;
            else if (word == "N") mat[i][j] = 0;
            else mat[i][j] = stoi(word);
        }
        for (int j=0; j<num_decisions; j++){
            fin >> word;
            if (!decisions[word]){
                decisions[word] = k;
                mat[i][j+num_attributes] = k++;
            } else {
                mat[i][j+num_attributes] = decisions[word];
            }
        }
    }

    for (auto u : decisions){
        inv_decisions[u.second] = u.first;
    }

    fin.close();
    return mat;
}

void printInfo (int** info, const int& num_objects, const int& num_attributes, 
                const int& num_decisions) {

    cout << "Decision table: " << endl << endl;
    for (int i=0; i<num_objects; i++){
        for (int j=0; j< (num_attributes); j++){
            cout << info[i][j] << " ";
        }
        for (int j=0; j<num_decisions; j++){
            cout << inv_decisions[info[i][j+num_attributes]];
        }
        cout << endl;
    }
    cout << endl;
}

void printEqClasses (vector<int> eq_classes[], int** eq_table, set<int> dec_classes[],
                     const int& num_eq, const int& num_attributes) {
    for (int i=0; i<num_eq; i++){
        for (auto u : eq_classes[i]){
            cout << "P" << u+1 << " ";
        }
        cout << setw(12 - eq_classes[i].size());
        for (int j=0; j<num_attributes; j++){
            cout << eq_table[i][j] << " ";
        }
        cout << setw(5);
        for (auto u : dec_classes[i]){
            cout << inv_decisions[u] << " ";
        }
        cout << endl;
    }
} 

void findApprox (vector<int> eq_classes[], set<int> dec_classes[], const int& num_eq, const int& num_decisions) {
    // Lower approx
    for (int i=0; i<num_decisions; i++){
        for (int j=0; j<inv_decisions.size(); j++){
            cout << "Lower approx for " << inv_decisions[j+1] << endl;

            for (int k=0; k<num_eq; k++){
                set<int>::iterator setIt = dec_classes[k].begin();
                if (dec_classes[k].size() == 1 && *setIt == j+1){
                    cout << "E" << k << " ";
                }
            }
            cout << endl;
        }
    }

    // Upper approx
    for (int i=0; i<num_decisions; i++){
        for (int j=0; j<inv_decisions.size(); j++){
            cout << "Upper approx for " << inv_decisions[j+1] << endl;

            for (int k=0; k<num_eq; k++){
                set<int>::iterator setIt = dec_classes[k].begin();
                while (setIt != dec_classes[k].end()){
                    if (*setIt == j+1){
                        cout << "E" << k << " ";
                        break;
                    }
                    setIt++;
                }
            }
            cout << endl;
        }
    }
}

int main () {
    inv_decisions.clear();
    int num_objects, num_attributes, num_decisions;
    int** info = getInput(num_objects, num_attributes, num_decisions);
    printInfo(info, num_objects, num_attributes, num_decisions);

    map<int, int> eq;
    int id = 1;
    for (int i=0; i<num_objects-1; i++){
        for (int j=i+1; j<num_objects; j++){
            bool same = true;
            for (int k=0; k<num_attributes; k++){
                if (info[i][k] != info[j][k]){
                    same = false;
                    break;
                }
            }
            if (same) {
                // cout << "P" << i+1 << " & P" << j+1 << " are same" << endl;
                if (!eq[i] && !eq[j]){
                    eq[i] = id;
                    eq[j] = id;
                    id++;
                } else if (eq[i] && !eq[j]){
                    eq[j] = eq[i];
                } else {
                    eq[i] = eq[j];
                }
            }
        }
    }

    // Alone objects
    for (int i=0; i<num_objects; i++){
        if (!eq[i])
        eq[i] = id++;
    }

    /*for (auto u : eq){
        cout << u.first+1 << " " << u.second << endl;
    }*/

    vector<int> eq_classes[id-1];
    for (auto u : eq){
        eq_classes[u.second-1].push_back(u.first);
       // cout << "Pushing P" << u.first+1 << " in " << u.second << endl; 
    }

    cout << "Equivalence classes: " << endl;
    for (int i=0; i<id-1; i++){
        cout << "E" << i << ": Objects ";
        for (auto u : eq_classes[i]){
            cout << u << " ";
        }
        cout << endl;
    }
    cout << endl;

    int** eq_table = new int*[id-1];
    set<int> dec_classes[id-1];
    for (int i=0; i<id-1; i++){
        eq_table[i] = new int[num_attributes + num_decisions];
        for (int j=0; j<num_attributes; j++){
            eq_table[i][j] = info[eq_classes[i][0]][j];
        }
        for (auto u : eq_classes[i]){
            for (int j=0; j<num_decisions; j++){
                dec_classes[i].insert(info[u][j + num_attributes]);
            }
        }
    }

    /*for (int i=0; i<id-1; i++){
        for (auto u : dec_classes[i]){
            cout << u << " ";
        }
        cout << endl;
    }*/
    
   // printEqClasses(eq_classes, eq_table, dec_classes, id-1, num_attributes);

    /*for (auto u : inv_decisions){
        cout << u.first << " " << u.second << endl;
    }*/

    findApprox(eq_classes, dec_classes, id-1, num_decisions); 
    return 0;
}