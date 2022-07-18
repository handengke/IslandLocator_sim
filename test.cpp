#include<iostream>
#include<vector>
#include<algorithm>
#include<omp.h>
#include<thread>
#include<bits/stdc++.h>
#include<sstream>
using namespace std;

void test(int& count){
    cout<<"i'm a thread! My id is "<<this_thread::get_id()<<endl;
    count+=1;
    cout<<"my count is "<<count<<endl;
}

void readcsv(string filename, vector<vector<int>>& edges){
    ifstream inFile(filename);
    string lineStr;
    while(getline(inFile,lineStr))
    {
        //cout<<"--"<<lineStr<<"--"<<endl;
        //将结果保存为二维表结构
        stringstream ss(lineStr);
        string str;
        vector<int> lineArray;
        //devide the numbers by "\t"
        while(getline(ss,str,'\t'))
        {
            istringstream ss(str);
            int temp=0;
            ss>>temp;
            lineArray.push_back(temp);
        }
        edges.push_back(lineArray); 
    }
}

struct Island
{
    vector<int> hub_list={};
    vector<int> island_nodes={};
    vector<int> shell_nodes={};

    Island(){};
    Island(vector<int> h,vector<int> n):hub_list{h},island_nodes(n){};
};


int main(){
    // vector<int> v {1,2,4,5,6};
    // remove(v.begin(),v.end(),4);
    // v.erase(find(v.begin(),v.end(),4));
    // for(auto e: v)
    //     cout<<e<<' ';

    // cout<<endl;
    // cout<<v.size()<<" "<<v.capacity();
    // vector<int> v1{1,3,4,5};
    // cout<<"the last number of v is: "<<v.back()<<endl;
    // if(v.front()==v1.front()) cout<<"YES!!"<<endl;
    // int i=5;
    // while (i--)
    // {
    //     vector<int> v;
    //     v.push_back(i);
    //     cout<<"the size of v is "<<v.size()<<endl;
    // }
    vector<int> a {1,2,3,4};
    vector<int> b {3,2,1,4};
    // Island newIsland(a,b);
    // for(auto e: newIsland.hub_list){
    //     cout<<e<<" ";
    // }
    // cout<<endl;
    // for(auto e: newIsland.island_nodes){
    //     cout<<e<<" ";
    // }
    // cout<<endl;    
    a.pop_back();
    for(auto e:a) cout<<e<<" ";
    cout<<a.size()<<endl;

    a.insert(a.begin(),2);

    for(auto e:a) cout<<e<<" ";
    cout<<a.size()<<endl;
    return 0;
//omp.cc
    // int count=0;
    // thread* th=new thread[4];
    // for(int i=0;i<4;i++){
    //     th[i]=thread(test,ref(count));
    // }
    //  for (int i = 0; i < 4; i++)
    //     th[i].join();//等待子线程结束
    // vector<vector<int>> edgList;
    // string filename="./test_file";
    // readcsv(filename,edgList);

    // for(auto e: edgList)
    // {
    //     cout<<e[0]<<"---"<<e[1]<<endl;
    // }

}