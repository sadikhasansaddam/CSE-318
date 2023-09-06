/*
--class values

unacc, acc, good, vgood

--attributes (6)

buying:   vhigh, high, med, low.
maint:    vhigh, high, med, low.
doors:    2, 3, 4, 5more.
persons:  2, 4, more.
lug_boot: small, med, big.
safety:   low, med, high
*/

#include <bits/stdc++.h>
using namespace std;

int totalAttribute = 6;

// Class to represent each example
class Example
{
private:
    vector<string> attributeList;
    string label;

public:
    Example()
    {
    }

    // setters
    void setAttributeList(vector<string> attributeList)
    {
        this->attributeList = attributeList;
    }

    void setLabel(string label)
    {
        this->label = label;
    }

    // getters
    vector<string> getAttributeList()
    {
        return this->attributeList;
    }

    string getLabel()
    {
        return this->label;
    }

    ~Example() {}
};

// Class to represent each tree node
class TreeNode
{
private:
    int attributeIndex; // index of the attribute
    string label;
    vector<pair<string, TreeNode>> childNodeList; // contains all the childNode of a node

public:
    TreeNode()
    {
    }

    // setters
    void setAttributeIndex(int attributeIndex)
    {
        this->attributeIndex = attributeIndex;
    }

    void setLabel(string label)
    {
        this->label = label;
    }

    void setChildNodeList(vector<pair<string, TreeNode>> childNodeList)
    {
        this->childNodeList = childNodeList;
    }

    // getters
    int getAttributeIndex()
    {
        return this->attributeIndex;
    }

    string getLabel()
    {
        return this->label;
    }

    vector<pair<string, TreeNode>> getChildNodeList()
    {
        return this->childNodeList;
    }

    // extra methods
    void addChildNode(string value, TreeNode childNode)
    {
        childNodeList.push_back(make_pair(value, childNode));
    }

    ~TreeNode() {}
};

// Function Declaration
vector<Example> getInputFromFile(vector<Example> &exampleSet);
void printElements(vector<Example> exampleSet);
TreeNode decisionTreeLearning(vector<Example> &exampleSet, vector<Example> &parentExampleSet, unordered_set<int> usedAttributeList);
string getPluralityLabel(vector<Example> exampleSet);
pair<bool, string> checkAllSampleLabel(vector<Example> exampleSet);
bool checkRemainingAttribute(unordered_set<int> usedAttributeList);
double getInformationGain(vector<Example> exampleSet, int attributeIndex);
double getEntropy(vector<Example> exampleSet);
unordered_map<string, int> getLabelCountMap(vector<Example> exampleSet);
string classifySample(TreeNode node, vector<string> attributeList);

// Function Definition

// This function builds a decision Tree given the Example Set
TreeNode decisionTreeLearning(vector<Example> &exampleSet, vector<Example> &parentExampleSet, unordered_set<int> usedAttributeList)
{
    TreeNode childNode;

    // If all the example has the Same label then return the childNode
    pair<bool, string> result = checkAllSampleLabel(exampleSet);
    bool flag = result.first;
    string label = result.second;
    if (flag == true)
    {
        childNode.setLabel(label);
        return childNode;
    }

    // if examples are empty then return the plurality value of exampleSet
    if (exampleSet.empty())
    {
        string pluralityLabel = getPluralityLabel(exampleSet);
        childNode.setLabel(pluralityLabel);
        return childNode;
    }

    // If there are no attributes left then return the plurality label of the exampleSet
    bool attributeLeft = checkRemainingAttribute(usedAttributeList);
    if (!attributeLeft)
    {
        string pluralityLabel = getPluralityLabel(exampleSet);
        childNode.setLabel(pluralityLabel);
        return childNode;
    }

    // Choosing the best attribute to split based on maximum information gain
    else
    {
        double maxInfoGain = -1.0;
        int bestAttribute = -1;

        for (int i = 0; i < totalAttribute; i++)
        {
            if (usedAttributeList.find(i) == usedAttributeList.end())
            {
                double currentInfoGain = getInformationGain(exampleSet, i);
                if (currentInfoGain > maxInfoGain)
                {
                    maxInfoGain = currentInfoGain;
                    bestAttribute = i;
                }
            }
        }

        childNode.setAttributeIndex(bestAttribute);
        usedAttributeList.insert(bestAttribute); // the attribute is now selected for split

        // we are iterating through the entire exampleSet to
        // find out the choosen attributes different value and
        // keep them in an unordered_map to store the individual count of them
        unordered_map<string, vector<Example>> splittedExample;
        for (auto sample : exampleSet)
        {
            vector<string> attributeList = sample.getAttributeList();
            string key = attributeList[bestAttribute];
            splittedExample[key].push_back(sample);
        }

        // For every value of the choosen attribute we create a subtree
        // and add this tree to the childrenList of the parent tree
        // and finally return the tree
        for (auto it : splittedExample)
        {
            string attributeValue = it.first;
            TreeNode node = decisionTreeLearning(it.second, exampleSet, usedAttributeList);
            childNode.addChildNode(attributeValue, node);
        }

        return childNode;
    }
}

// This function checks the entire example set to find out if all the example has the same label or not
pair<bool, string> checkAllSampleLabel(vector<Example> exampleSet)
{
    bool sameLabel = true;
    string firstLabel = exampleSet[0].getLabel();
    for (auto row : exampleSet)
    {
        if (row.getLabel() != firstLabel)
        {
            sameLabel = false;
            break;
        }
    }

    pair<bool, string> result;
    result.first = sameLabel;
    result.second = firstLabel;

    return result;
}

// This function checks whether there is any attribute left to split or not
bool checkRemainingAttribute(unordered_set<int> usedAttributeList)
{
    bool attributeLeft = true;
    if (usedAttributeList.size() == totalAttribute)
    {
        attributeLeft = false; // no attributes left to split
    }
    else
    {
        attributeLeft = true; // all attributes are remaining, we can split
    }
    return attributeLeft;
}

// This function calculates information gain for splitting particular attribute
double getInformationGain(vector<Example> exampleSet, int attributeIndex)
{
    double beforeSplitEntropy, afterSplitEntropy, informationGain;
    beforeSplitEntropy = getEntropy(exampleSet);

    // we are iterating through the entire exampleSet to
    // find out the attributes different value and
    // keep them in an unordered_map to store the individual count of them

    unordered_map<string, vector<Example>> splittedExample;
    for (auto sample : exampleSet)
    {
        vector<string> attributeList = sample.getAttributeList();
        string key = attributeList[attributeIndex];
        splittedExample[key].push_back(sample);
    }

    // For every value of the attribute we calculate entropy and add it to afterSplitEntropy
    // Finally we return the information gain which is the difference of beforeSplitEntropy and afterSplitEntropy
    int exampleSetSize = exampleSet.size();
    afterSplitEntropy = 0.0;
    for (auto it : splittedExample)
    {
        int particularAttributeValueCount;
        double weight, entropy;

        particularAttributeValueCount = it.second.size();
        weight = static_cast<double>(particularAttributeValueCount) / static_cast<double>(exampleSet.size());
        entropy = getEntropy(it.second);
        afterSplitEntropy += weight * entropy;
    }

    informationGain = beforeSplitEntropy - afterSplitEntropy;
    return informationGain;
}

// This function calculates entropy for a particular attribute value
double getEntropy(vector<Example> exampleSet)
{
    // labelCountMap stores the labelCount for any particular label of a given exampleSet
    unordered_map<string, int> labelCountMap = getLabelCountMap(exampleSet);

    // calculates entropy for all the different class label and return the value
    int exampleSetSize = exampleSet.size();
    double entropy = 0.0;
    for (auto it : labelCountMap)
    {
        int labelCount = it.second;
        double probability = static_cast<double>(labelCount) / static_cast<double>(exampleSetSize);
        entropy -= probability * log2(probability);
    }

    return entropy;
}

// This function returns the plurality label of a given example set
string getPluralityLabel(vector<Example> exampleSet)
{
    // labelCountMap stores the labelCount for any particular label of a given exampleSet
    unordered_map<string, int> labelCountMap = getLabelCountMap(exampleSet);

    // Iterates through the labelCountMap to find
    //  which label appeared maximum number of times and returns the value
    int maxLabelCount = 0;
    string majorityLabel;
    for (auto it : labelCountMap)
    {
        if (it.second > maxLabelCount)
        {
            maxLabelCount = it.second;
            majorityLabel = it.first;
        }
    }

    return majorityLabel;
}

// This function calculates different label's count for a particular exampleSet
unordered_map<string, int> getLabelCountMap(vector<Example> exampleSet)
{
    // count how many examples are for any particular class label
    // and store it in a map called labelCountMap
    unordered_map<string, int> labelCountMap;
    for (auto sample : exampleSet)
    {
        string label = sample.getLabel();
        labelCountMap[label]++;
    }

    return labelCountMap;
}

// This function iterates through a given tree to find out the class label
string classifySample(TreeNode node, vector<string> attributeList)
{
    // If it is a leaf node then return its label
    if (node.getChildNodeList().empty())
    {
        return node.getLabel();
    }

    // otherwise iterate through its subtree
    int attributeIndex = node.getAttributeIndex();
    string attributeValue = attributeList[attributeIndex];
    bool found = false;
    int index;
    for (int i = 0; i < node.getChildNodeList().size(); i++)
    {
        if (node.getChildNodeList()[i].first == attributeValue)
        {
            found = true;
            index = i;
            break;
        }
    }

    if (found)
    {
        return classifySample(node.getChildNodeList().at(index).second, attributeList);
    }
    else
    {
        return node.getLabel();
    }
}

// This function prints all the example
void printElements(vector<Example> exampleSet)
{
    // Using an iterator
    for (auto it = exampleSet.begin(); it != exampleSet.end(); ++it)
    {
        // printing attribute List
        vector<string> attributeList = it->getAttributeList();
        for (int i = 0; i < attributeList.size(); i++)
        {
            cout << attributeList[i] << ",";
        }
        cout << it->getLabel() << endl;
    }
}

// This function reads data from a input file
vector<Example> getInputFromFile(string fileName, vector<Example> &exampleSet)
{
    ifstream fin(fileName);
    if (!fin.is_open())
    {
        cout << "Error: Unable to open file " << fileName << endl;
    }

    vector<string> row;
    string line;

    while (getline(fin, line))
    {
        row.clear();
        Example sample;
        string token;
        istringstream ss(line);
        while (getline(ss, token, ','))
        {
            if (ss.peek() == '\n')
            {
                token.erase(remove(token.begin(), token.end(), '\n'), token.end());
            }
            if (ss.peek() == '\r')
            {
                token.erase(remove(token.begin(), token.end(), '\r'), token.end());
            }
            if (token == "unacc" || token == "acc" || token == "good" || token == "vgood")
            {
                sample.setLabel(token);
            }
            else
            {
                row.push_back(token);
            }
        }

        sample.setAttributeList(row);
        exampleSet.push_back(sample);
    }

    return exampleSet;
}

// driver function
int main()
{
    vector<Example> exampleSet, parentExampleSet;
    int experimentCount = 20, exampleSetSize, trainingDataSize, testDataSize;
    float trainingDataRatio = 0.8, testDataRatio;
    vector<double> accuracyList;
    string fileName = "car.data";

    exampleSet = getInputFromFile(fileName, exampleSet);
    testDataRatio = 1 - trainingDataRatio;
    exampleSetSize = exampleSet.size();
    trainingDataSize = static_cast<int>(trainingDataRatio * exampleSet.size());
    testDataSize = exampleSetSize - trainingDataSize;

    for (int i = 0; i < experimentCount; i++)
    {
        // Shuffle the exampleSet and then split into training and testing exampleSet
        // random_shuffle(exampleSet.begin(), exampleSet.end());

        random_device rd;
        mt19937 g(rd());
        shuffle(exampleSet.begin(), exampleSet.end(), g);

        vector<Example> trainingExampleSet(exampleSet.begin(), exampleSet.begin() + trainingDataSize);
        vector<Example> testExampleSet(exampleSet.begin() + trainingDataSize, exampleSet.end());
        unordered_set<int> usedAttributeList;
        TreeNode rootNode;
        rootNode = decisionTreeLearning(trainingExampleSet, parentExampleSet, usedAttributeList);

        // Testing the decision tree on the testing data
        int correctPredictionCount = 0;
        for (auto sample : testExampleSet)
        {
            string predictedLabel = classifySample(rootNode, sample.getAttributeList());
            if (predictedLabel == sample.getLabel())
            {
                correctPredictionCount++;
            }
        }

        double accuracy = (static_cast<double>(correctPredictionCount) / static_cast<double>(testDataSize)) * 100;
        // cout << "Experiment no  : " << i + 1 << "  Acuracy : " << accuracy << endl;

        accuracyList.push_back(accuracy);
    }

    // Calculate and report average accuracy, mean accuracy, and standard deviation
    double totalAccuracyValue = accumulate(accuracyList.begin(), accuracyList.end(), 0.0);
    double averageAccuracy = totalAccuracyValue / experimentCount;

    double squaredDifferencesSum = 0.0;
    for (double accuracy : accuracyList)
    {
        squaredDifferencesSum += (accuracy - averageAccuracy) * (accuracy - averageAccuracy);
    }
    double variance = squaredDifferencesSum / experimentCount;
    double standardDeviation = sqrt(variance);

    cout << "\nAverage Test Accuracy: " << averageAccuracy << "%" << endl;
    cout << "Standard Deviation of Accuracy: " << standardDeviation << endl;

    return 0;
}
