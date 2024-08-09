// libraries needed for DS
#include "Trees.cpp"
// libraries needed for testing
#include <iostream>
#include <map>
#include <vector>
#include <random>
#include <math.h>
#include <chrono>

using namespace std;

vector<string> generate(int num, int max) {
    vector<string> strings;
    for (int i = 1; i < num; i++)
    {
        string to_add = to_string(i);
        while (to_add.size() != 8)
            to_add.insert(to_add.begin(), '0');
        int num = rand() % max + 1;
        while (num--)
            strings.push_back(to_add);
    }

    random_shuffle(strings.begin(), strings.end());
    return strings;
}

vector<string> get_anss(vector<string> &strings) {
    int maxir = 0;
    string ansr;
    map<string, int> real;
    vector<string> out;
    for (int i = 0; i < strings.size(); i++) {
        string s = strings[i];

        real[s]++;
        if (real[s] > maxir) {
            ansr = s;
            maxir = real[s];
        }

        out.push_back(ansr);
    }
    return out;
}

int grade_noisy(vector<string> &strings, vector<string> &ans, bool mode) {
    int maxi1 = 0;
    string ans1;
    int num = 0;
    NoisyRedBlackTree tre;
    int num_rep = ceil(log2(strings.size()));
    tre.set_num_rep(num_rep);
    tre.set_mode(mode);
    for (int i= 0; i < strings.size(); i++) {
        string s = strings[i];
        Node* nod = tre.insert(s, 0);

        if (nod != nullptr) {
                nod->value++;
            if (nod->value > maxi1)
                ans1 = s, maxi1 = nod->value;
        }
        
        num += (int)(ans[i] != ans1);
    }
    return num;
}

int grade_boosting(vector<string> &strings, vector<string> &ans) {
    int maxi1 = 0;
    string ans1;
    int num = 0;
    BoostingRedBlackTree tre;
    int num_rep = ceil(log2(strings.size()));
    tre.set_num_rep(num_rep);
    for (int i= 0; i < strings.size(); i++) {
        string s = strings[i];
        Node* nod = tre.insert(s, 0);

        if (nod != nullptr) {
            nod->value++;
            if (nod->value > maxi1)
                ans1 = s, maxi1 = nod->value;
        }
        
        num += (int)(ans[i] != ans1);
    }
    return num;
}

void mfs_testing() {
    int T = 1;
    double avg_len = 0;
    int maxi = 4000, num = 400;
    double tot = 0;
    double avg_error = 0;
    double avg_query_num = 0;

    double tot2 = 0;
    double avg_error2 = 0;
    double avg_query_num1 = 0;

    double tot3 = 0;
    double avg_error3 = 0;
    double avg_query_num2 = 0;
    for (int t = 0; t < T; t++) {
        vector<string> strings = generate(num, maxi);
        vector<string> ans = get_anss(strings);
        avg_len += (double) strings.size() / (double)T;

        reset_num_query();
        auto start = std::chrono::system_clock::now();
        avg_error += (double)grade_noisy(strings, ans, false) / (double)T;
        auto end = std::chrono::system_clock::now();
        auto elapsed = end - start;
        tot += elapsed.count() * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;
        avg_query_num += (double)get_num_query() / (double)T;

        reset_num_query();
        start = std::chrono::system_clock::now();
        avg_error2 += (double)grade_boosting(strings, ans) / (double)T;
        end = std::chrono::system_clock::now();
        elapsed = end - start;
        tot2 += elapsed.count() * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;
        avg_query_num1 += (double)get_num_query() / (double)T;

        reset_num_query();
        start = std::chrono::system_clock::now();
        avg_error3 += (double)grade_noisy(strings, ans, true) / (double)T;
        end = std::chrono::system_clock::now();
        elapsed = end - start;
        tot3 += elapsed.count() * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;
        avg_query_num2 += (double)get_num_query() / (double)T;
    }
    cout << avg_len << endl;

    cout << "NOISY" << endl;
    cout << "TIME: " << tot << endl;
    cout << "AVG. ERROR:" << avg_error << endl;
    cout << "AVG. ERROR PERECENT: " << avg_error / avg_len * 100 << "%" << endl;
    cout << "AVG. QUERY NUM: " << avg_query_num << endl;
    cout << endl;

    cout << "BOOSTING" << endl;
    cout << "TIME: " << tot2 << endl;
    cout << "AVG. ERROR:" << avg_error2 << endl;
    cout << "AVG. ERROR PERECENT: " << avg_error2 / avg_len * 100 << "%" << endl;
    cout << "AVG. QUERY NUM: " << avg_query_num1 << endl;
    cout << endl;

    cout << "NOISY2" << endl;
    cout << "TIME: " << tot3 << endl;
    cout << "AVG. ERROR:" << avg_error3 << endl;
    cout << "AVG. ERROR PERECENT: " << avg_error3 / avg_len * 100 << "%" << endl;
    cout << "AVG. QUERY NUM: " << avg_query_num2 << endl;
    cout << endl;
}

int main() {
    srand(time(0));
    
    vector<string> strings = {"01", "00", "04", "03"};

    mfs_testing();

    return 0;
}