#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// declare node
struct Node {
  int data1;
  int data2;
  Node *next;
  Node(int num1, int num2) : data1(num1), data2(num2), next(nullptr) {}
};

vector<Node> nodes;

void addNode(int num1, int num2) {
  nodes.emplace_back(num1, num2);
  nodes.emplace_back(num2, num1);
}

ifstream open(const string &src_file) {
  ifstream src{src_file};
  // if statement to check whether file is open or not.
  if (!src.is_open()) {
    cout << "Could not find file " << src_file << "." << endl;
    exit(1);
  }
  return src;
}

// This function reads the file and adds each line as a node into the linked
// list.
void read_file(string filename) {
  ifstream file;
  file = open(filename);

  string line;
  while (getline(file, line)) {
    istringstream ss(line);

    int num1;
    int num2;

    ss >> num1 >> num2;

    addNode(num1, num2);
  }
}

void display_all_nodes() {
  cout << "The list contains the data entered: " << endl;
  for (int i = 0; i < nodes.size(); i++) {
    Node temp = nodes[i];
    cout << temp.data1 << ": " << temp.data2 << endl;
  }
  cout << endl;
}

bool compareNodes(const Node &node1, const Node &node2) {
  if (node1.data1 == node2.data1) {
    return node1.data2 < node2.data2;
  }
  return node1.data1 < node2.data1;
}

void sort() { sort(nodes.begin(), nodes.end(), compareNodes); }

string makeFileName(string filename) {
  string graphFile = filename;
  for (int i = 0; i < 4; i++) {
    graphFile.pop_back();
  }
  graphFile += "_adj.txt";
  return graphFile;
}

void remove_duplicates() {
  auto last =
      unique(nodes.begin(), nodes.end(), [](const Node &a, const Node &b) {
        return (a.data1 == b.data1 && a.data2 == b.data2);
      });
  nodes.erase(last, nodes.end());
}

string nodes_counter() {
  string val;
  int counter = 1;
  for (int i = 1; i < nodes.size(); i++) {
    if (nodes[i - 1].data1 < nodes[i].data1) {
      counter++;
    }
  }
  val = "Number of nodes: " + to_string(counter);
  return val;
}

string edges_counter() {
  string val;
  int counter = nodes.size() / 2;
  val = "Number of edges: " + to_string(counter);
  return val;
}

string max_counter() {
  string val;
  int counter = 1;
  int temp = 0;
  for (int i = 1; i < nodes.size(); i++) {
    if (nodes[i - 1].data1 == nodes[i].data1) {
      counter++;
    } else {
      counter = 1;
    }
    if (counter > temp) {
      temp = counter;
    }
  }
  val = "Maximum degree: " + to_string(temp);
  return val;
}

void makeFile(const string &filename) {
  ofstream fstr(filename);
  int curr_node = nodes[0].data1;
  fstr << curr_node << ":";
  for (const Node &node : nodes) {
    if (node.data1 != curr_node) {
      curr_node = node.data1;
      fstr << endl << curr_node << ":";
    }
    fstr << " " << node.data2;
  }
  fstr << endl;
  fstr.close();
}

void run(const string &src_path, const string &dst_path) {
  read_file(src_path);
  sort();
  remove_duplicates();
  // display_all_nodes();
  cout << nodes_counter() << endl;
  cout << edges_counter() << endl;
  cout << max_counter() << endl;

  makeFile(dst_path);
}

int main() {
  string filename;
  cout << "Enter file you want to graph: " << endl;
  cin >> filename;
  filename += ".txt";

  string graphFile = makeFileName(filename);
  run(filename, graphFile);
}