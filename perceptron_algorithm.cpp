#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <time.h>
#include <stdlib.h>


using namespace std;

vector<float> MinAndMax(vector<vector<float>>& input) {
   float min_val = input[0][0];
   float max_val = input[0][1];
   int len = input.size();
   for (int idx = 0; idx < len; idx++) {
       float cur_min = min(input[idx][0], input[idx][1]);
       float cur_max = max(input[idx][0], input[idx][1]);
       min_val = min(cur_min, min_val);
       max_val = max(cur_max, max_val);
   }
   vector<float> res{min_val, max_val};
   return res;
}

// given a line x1,x2,label
// return a vector of float
vector<float> ParseLine(string& input) {
  int pos = 0;
  int idx = input.find(",", pos);
  vector<float> res;
  while(idx != -1) {
     //convert the string to float
  	 string component = input.substr(pos, idx-pos);
  	 float val = stof(component);
  	 res.push_back(val);
  	 pos = idx + 1;
     idx = input.find(",", pos);
  }
  //the last component
  string last = input.substr(pos);
  float last_val = stof(last);
  res.push_back(last_val);
  return res;
}

// Given a filename as input
// read line by line into memory
// format of file:
// x1,x2,label
vector<vector<float>> ReadData(
	                   const string& filename) {
  vector<vector<float>> res;
  fstream fp;
  fp.open(filename.c_str(), ios::in);
  if (fp.is_open()) {
     string line;
     while(getline(fp, line)) {
     	vector<float> temp = ParseLine(line);
     	//cout<< "Debug:" << temp[0] << ","
     	//    << temp[1] << "," << temp[2] << endl;
     	res.push_back(temp); 
     }
  }
  else {
  	exit(-1);

  }
  return res;
}

void OutputModel(vector<vector<float>>& history) {
    ofstream fp;
    fp.open("./model.txt");
    int len = history.size();
    for(int idx = 0; idx < len; idx++) {
        fp << history[idx][0] << ","
           << history[idx][1] << "," 
           << history[idx][2] << ","
           << history[idx][3] << endl;
    }
    fp.close();

}

// Step function to convert a input to 1 or 0
int StepFunc(float input) {
	if (input > 0) {
		return 1;
	}
	else {
		return 0;
	}
}

int Predict(vector<float>& feature,
            vector<float>& params) {
  
  float res = feature[0] * params[0]
              + feature[1] * params[1]
              + params[2];

  return StepFunc(res);

}


vector<float> GenRand() {
	vector<float> randoms;
    float w1 = (float)rand() / RAND_MAX;
    float w2 = (float)rand() / RAND_MAX;
    float bias = (float)rand() / RAND_MAX;
    randoms.push_back(w1);
    randoms.push_back(w2);
    randoms.push_back(bias);
    cout<< "random value:" 
        << w1 << " " 
        << w2 << " " 
        << bias << endl;
    return randoms;
}

// a single step of learning
vector<float> PerceptronStep(vector<vector<float>>& data,
                             vector<float>& params,
                             float learn_rate) {
   int data_size = data.size();
   int errors = 0;
   vector<float> res;
   for (int idx = 0; idx < data_size; idx++) {
      float actual_label = data[idx][2];
      vector<float> feature{data[idx][0], data[idx][1]};
      float predict_label = Predict(feature, params);
      if(predict_label == actual_label) {
      	 continue;
      }
      float diff = predict_label - actual_label;
      errors++;
      // two cases predict = 1 and actual = 0,
      // lower the weight to make that predict lower
      if (diff == 1) {
         params[0] = params[0] - learn_rate * data[idx][0];
         params[1] = params[1] - learn_rate * data[idx][1];
         params[2] = params[2] - learn_rate;

      }
      else if (diff == -1) {
      	 // predict = 0 and label == 1,
      	 // add the weight to make that predict higher
         params[0] = params[0] + learn_rate * data[idx][0];
         params[1] = params[1] + learn_rate * data[idx][1];
         params[2] = params[2] + learn_rate;
      }
   }
   cout<<params[0] << ","
    	 << params[1]
    	 << ","
    	 << params[2] << endl;
   cout<<"Errors:"<<errors<<endl;
   res.push_back(params[0]);
   res.push_back(params[1]);
   res.push_back(params[2]);
   res.push_back(errors);
   return res;
}

// w1*x1 + w2*x2 + bias = label
vector<vector<float>> Train(vector<vector<float>>& data, float learn_rate = 0.01, int num_epochs = 25) {
  vector<float> min_max = MinAndMax(data);
  vector<float> params = GenRand();
  vector<vector<float>> history;
  params[2] = min_max[1] + params[2];
  params.push_back(INT_MAX);
  for(int epoch = 0; epoch < num_epochs; epoch++) {
  	cout << "Train for epoch:" << epoch + 1 << endl;
  	params = PerceptronStep(data, params, learn_rate);
    history.push_back(params);
  }
  return history;
}

int main() {
    srand(time(NULL));
    vector<vector<float>> data = ReadData("./data.csv");
    
    vector<vector<float>> history = Train(data, 0.1, 40);
    OutputModel(history);

	return 0;
}