#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

using namespace std;

int n, m, i = 0, j, k, l;
char a, b, c, d, Conv = 'a';

vector<string> TempT, TempH, TempV;
vector<char> TailAdjList, HeadAdjList, AdjVertList;
vector<int> AdjEdgList;
vector<vector<int>> AdjMatrix, TmpMatrix, OrgMatrix;
map<char, string> TailDict, HeadDict;///lista poprzedników i nastepnikow

bool IsAdjoint = false;
bool IsMulti = false;
bool IsLinear = true;
int HasIsol = 0; ///it is obvious

ifstream input;
ofstream output;

int binomial(int n, int k)
{
    int num, den ;
    if ( n < k ) return(0) ;

    else
    {
        den = 1;
        num = 1 ;
        for (int i =  1  ; i <= k   ; i = i+1) den = den * i;
        for (int j = n-k+1; j<=n; j=j+1) num = num * j;
        return(num/den);
    }
}

int main()
{
///FILE READ///
    input.open("in.txt", ios::in | ios::binary);///using a file in read-only mode former test8
    string line;
    if(!input.good())
    {
        cout<<"Blad otwarcia"<<endl;
        return 1; ///when something goes wrong
    }
    while(input.good())///filling a matrix x and y axis and filling a tail and head container
    {
        getline(input, line);

        if(line.size() > 1)
        {
            auto it = find(line.begin(), line.end(), '>');///seeking separator
            int x = distance(line.begin(), it);///counting where it is
            int y = line.size();
            int z = (y - x) - 2;///because we miss '>' and '\n' chars

            if(y == x + 2)
            {
                TempT.push_back(line.substr(0, x));///isolated
                TailAdjList.push_back(Conv);
            }
            else
            {
                TempT.push_back(line.substr(0, x));///taking x chars from 0
                TempH.push_back(line.substr(x + 1, z));///taking x chars from x position
                TailAdjList.push_back(Conv);
                HeadAdjList.push_back(Conv);
            }
            m++;
            AdjEdgList.push_back(m);
        }
    }
    input.close();
///END OF FILE READ///



///MAKING A MATRIX AND LISTS///

    TempV.insert(TempV.end(), TempT.begin(), TempT.end());
    TempV.insert(TempV.end(), TempH.begin(), TempH.end());

    sort(TempV.begin(), TempV.end());///sorting this container
    auto iter = unique(TempV.begin(), TempV.end());
    TempV.erase(iter, TempV.end());
    n = TempV.size();

    for(i = 0; i < TempV.size(); i++)
    {
        for(j = 0; j < TempT.size(); j++)
        {
            if(TempT[j] == TempV[i]) TailAdjList[j] = (Conv + i);
        }

        for(j = 0; j < TempH.size(); j++)
        {
            if(TempH[j] == TempV[i]) HeadAdjList[j] = (Conv + i);
        }
    }

    AdjVertList.insert(AdjVertList.end(), TailAdjList.begin(), TailAdjList.end());///putting all vertices in yet not sorted container
    AdjVertList.insert(AdjVertList.end(), HeadAdjList.begin(), HeadAdjList.end());

    sort(AdjVertList.begin(), AdjVertList.end());///sorting this container
    auto last = unique(AdjVertList.begin(), AdjVertList.end());
    AdjVertList.erase(last, AdjVertList.end());
    n = AdjVertList.size();

    for(i = 0; i < n; i++) AdjMatrix.push_back(vector<int>(m, 0));///filling matrix with zeros

    for(i = 0; i < n; i++)///filling lists with 'zeros'
    {
        TailDict[AdjVertList[i]] = "";
        HeadDict[AdjVertList[i]] = "";
    }

    for(i = 0; i < TailAdjList.size(); i++)///iterate over m (or m+ if theres isolated)
    {
        for(j = 0; j < AdjVertList.size(); j++)///iterate over n
        {
            if(AdjVertList[j] == TailAdjList[i])
            {
                AdjMatrix[j][i] = -1;///tail in a matrix
                line = HeadDict[AdjVertList[j]];///and by occassion 'lista nastepnikow'
                line += HeadAdjList[i];
                sort(line.begin(), line.end());
                HeadDict[AdjVertList[j]] = line;
            }
            if(AdjVertList[j] == HeadAdjList[i])
            {
                AdjMatrix[j][i] = 1;///head in a matrix
                line = TailDict[AdjVertList[j]];///and by occasion 'lista poprzednikow'
                line += TailAdjList[i];
                sort(line.begin(), line.end());
                TailDict[AdjVertList[j]] = line;
            }
            if(HeadAdjList[i] == TailAdjList[i])///self
            {
                vector<char> ::iterator it = find(AdjVertList.begin(), AdjVertList.end(), TailAdjList[i]);
                k = (it - AdjVertList.begin());
                AdjMatrix[k][i] = 4;
            }
        }
    }
    if(TailAdjList.size() > HeadAdjList.size())///isolated vertice
    {
        HasIsol = (TailAdjList.size() - HeadAdjList.size());
        k = (TailAdjList.size() - HeadAdjList.size());

        for(j = 0; j < HasIsol; j++)///and erase elements from a map and modify a vector
        {
            d = (TailAdjList.back() -j);
            auto it = find(AdjVertList.begin(), AdjVertList.end(), d);
            int x = distance(AdjVertList.begin(), it);

            AdjMatrix[x][m - j - 1] = 0;
            TailDict.erase(d);
            HeadDict.erase(d);
        }
    }
///END OF MAKING A MATRIX///
/*
cout<<endl;
    for(i = 0; i < m; i++) cout<<"\t"<<i + 1;

    for(i = 0; i < n; i++)///printing a proper incidence matrix(-1 is a beginning, 1 is end of a edge, 4 is self)
    {
        cout<<endl;
        cout<<i + 1<<"\t";
        for(j = 0; j < m; j++) cout<<AdjMatrix[i][j]<<"\t";
    }
*/
///CHECKING EXCEPTIONS///


    vector<string> tmp, tmp2;///help in comparing strings

    int MoreAdjoint = 0;

    for(map<char, string> ::iterator it = HeadDict.begin(); it != HeadDict.end(); ++it) tmp.push_back(it->second);

    for(i = 0; i < tmp.size(); i++)///check adjointnessssss
    {
        string TmpLine1 = tmp[i];
        set<char> cnt(TmpLine1.begin(), TmpLine1.end());///check multigraphnessssss
        if(cnt.size() != TmpLine1.size()) IsMulti = true;

        for(j = i + 1; j < tmp.size(); j++)
        {
            int x = 0;
            string TmpLine2 = tmp[j];

            for(auto it = TmpLine1.begin(); it != TmpLine1.end(); ++it)
            {
                for(auto it2 = TmpLine2.begin(); it2 != TmpLine2.end(); ++it2)
                {
                    if(*it2 == *it) x++;
                }
            }
            if(x == 0 || x == TmpLine1.size() || x == TmpLine2.size()) MoreAdjoint++;
        }
    }

    k = tmp.size();
    int bin = binomial(k, 2);///a key for adjoint confirmation


    if(bin == MoreAdjoint) IsAdjoint = true;

    if(IsAdjoint)
    {
        for(i = 0; i < tmp.size(); i++)///check linenessssss
        {
            int counter = count(tmp.begin(), tmp.end(), tmp[i]);///here we check if it's an adjoint simply by counting occurence
            map<char, string> ::iterator it4 = TailDict.find('a');///defining an empty iterator to help in line check

            if(counter > 1)
            {
                line = tmp[i];
                vector<string> ::iterator it = tmp.begin();///just first of
                tmp2.clear();

                for(j = 0; j < counter; j++)
                {
                    vector<string> ::iterator it2 = find(it, tmp.end(), line);
                    map<char, string> ::iterator it3 = HeadDict.begin();
                    k = (it2 - tmp.begin()) + 1;///we have to be sure we dont choose the same element again, thats why we increment .begin() iterator
                    advance(it, k);
                    advance(it3, k - 1);

                    tmp2.push_back(TailDict.find(it3->first)->second);///we stack here a pack of tails to check if the condition is correct

                    int counter2 = count(tmp2.begin(), tmp2.end(), tmp2.back());///check if they are the same

                    if(counter2 > 1 && IsLinear == true) IsLinear = false;
                }
            }
        }
    }

    if(IsMulti)
    {
        cout<<"Graf jest multigrafem. Koncze prace..."<<endl;
        return 1;
    }
    if(IsAdjoint) cout<<"To jest graf sprzezony."<<endl;
    else
    {
        cout<<"Graf nie jest sprzezony. Koncze prace..."<<endl;
        return 1;
    }
    if(IsLinear) cout<<"To jest graf liniowy."<<endl;
    else cout<<"Graf jest nieliniowy."<<endl;


/*
    cout<<endl;
    for(auto it = HeadDict.begin(); it != HeadDict.end(); ++it) cout<<endl<<it->first<<" -> "<<it->second;
    cout<<endl;
    for(auto it = TailDict.begin(); it != TailDict.end(); ++it) cout<<endl<<it->first<<" -> "<<it->second;

*/









///CAN WE CONVERT NOW???///

    for(i = 0; i < m; i++) TmpMatrix.push_back(vector<int>(n, 0));

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++) TmpMatrix[j][i] = -AdjMatrix[i][j];///first 'conversion'
    }


    vector<int> TmpCol;
    multimap<int, int> RowsToReduce;

    int cnt;

    for(i = 0; i < n; i++)///checking which rows should be merged
    {
        TmpCol.clear();

        for(j = 0; j < m; j++) TmpCol.push_back(TmpMatrix[j][i]);

        cnt = count(TmpCol.begin(), TmpCol.end(), -1);///counting minus
        auto src = find(TmpCol.begin(),TmpCol.end(), -4);///we dont want to accidentaly delete a self edge

        if((cnt > 1) && (src == TmpCol.end()))
        {
            auto it = TmpCol.begin();

            for(k = 0; k < cnt; k++)
            {
                auto it2 = find(it, TmpCol.end(), -1);
                it = it2;
                advance(it, 1);
                if(k == 0) l = (it2 - TmpCol.begin()); ///we must know where to insert
                else RowsToReduce.insert(make_pair((it2 - TmpCol.begin()), l));
            }
        }

        cnt = count(TmpCol.begin(), TmpCol.end(), 1);///counting plus
        src = find(TmpCol.begin(),TmpCol.end(), -4);///we dont want to accidentaly delete a self edge

        if((cnt > 1) && (src == TmpCol.end()))
        {
            auto it = TmpCol.begin();///where to;

            for(k = 0; k < cnt; k++)
            {
                auto it2 = find(it, TmpCol.end(), 1);
                it = it2;
                advance(it, 1);
                if(k == 0) l = (it2 - TmpCol.begin());///where to
                else RowsToReduce.insert(make_pair((it2 - TmpCol.begin()), l));
            }
        }
    }

    for(auto it3 = RowsToReduce.begin(); it3 != RowsToReduce.end(); ++it3)///reapirning map
    {
        for(auto it4 = RowsToReduce.begin(); it4 != RowsToReduce.end(); ++it4)
        {
            if(it3->first == it4->second)
            {
                RowsToReduce.insert(make_pair(it4->first, it3->second));
                RowsToReduce.erase(it4);
            }
            else if(it3->first == it4->first && it3->second != it4->second)
            {
                if(it3->second > it4->second)
                {
                    RowsToReduce.insert(make_pair(it3->second, it4->second));
                    RowsToReduce.erase(it3);
                }
                else if(it3->second < it4->second)
                {
                    RowsToReduce.insert(make_pair(it4->second, it3->second));
                    RowsToReduce.erase(it4);
                }
            }
        }
        cnt = RowsToReduce.count(it3->first);
        if(cnt > 1) RowsToReduce.erase(it3);
    }


    vector<int> ToDelete;

    for(auto rit = RowsToReduce.rbegin(); rit != RowsToReduce.rend(); ++rit)
    {
        int x = rit->second;///assign a value
        int y = rit->first;///here too
        ToDelete.push_back(y);

        for(i = 0; i < n; i++)
        {
            if(TmpMatrix[x][i] == 0 && TmpMatrix[y][i] != 0) TmpMatrix[x][i] = TmpMatrix[y][i];///changing values
        }
    }


    for(i = 0; i < n; i++)///self-edge remaking in matrix
    {
        TmpCol.clear();

        for(j = 0; j < m; j++) TmpCol.push_back(TmpMatrix[j][i]);

        cnt = count(TmpCol.begin(), TmpCol.end(), -4);///and self-edge search
        auto it2 = find(TmpCol.begin(), TmpCol.end(), -4);///no sense if -4 is at the beginning

        if((cnt == 1) && (it2 != TmpCol.begin()))
        {
            l = i;///we need to know which column
            auto it = find(TmpCol.begin(), TmpCol.end(), -1);///where to, just first
            k = distance(TmpCol.begin(), it);
            it = find(TmpCol.begin(), TmpCol.end(), 1);///where to

            if((distance(TmpCol.begin(), it)) < k) k = distance(TmpCol.begin(), it);///need to know what is first, 1 or -1

            fill(TmpCol.begin(), TmpCol.end(), 0);
            TmpCol[k] = 4;
            for(it = TmpCol.begin(); it != TmpCol.end(); ++it)
            {
                int x = distance(TmpCol.begin(), it);
                TmpMatrix[x][i] = *it;
            }
        }
    }


    sort(ToDelete.rbegin(), ToDelete.rend());///reverse sorting this container because we might delete something useful  during resizing
    auto it = unique(ToDelete.begin(), ToDelete.end());///just in case
    ToDelete.erase(it, ToDelete.end());


    for(auto it = ToDelete.begin(); it != ToDelete.end(); ++it)
    {
        i = *it;
        TmpMatrix.erase(TmpMatrix.begin() + i);///deleting a bad row and resizing, we also have to delete the items from the map
        m--; ///it is obvious
    }


    vector<int> TmpRow;
    tmp.clear();

    for(i = 0; i < n; i++)///if there are some blind ways and if we have isolated
    {
        TmpCol.clear();

        for(j = 0; j < m; j++)
        {
            TmpRow.clear();
            TmpCol.push_back(TmpMatrix[j][i]);
            TmpRow.insert(TmpRow.end(), TmpMatrix[j].begin(), TmpMatrix[j].end());
            int counter = count(TmpRow.begin(), TmpRow.end(), 0);
        }
        cnt = count(TmpCol.begin(), TmpCol.end(), 0);

        if(cnt != (m - 2) && cnt != m)///the must be only two ints other than zero
        {
            auto it = find(TmpCol.begin(), TmpCol.end(), -1);

            if(it == TmpCol.end());///this we want not
            else
            {
                m++;///add a row again...
                TmpMatrix.push_back(vector<int>(n, 0));
                TmpMatrix[m - 1][i] = 1;
            }

            it = find(TmpCol.begin(), TmpCol.end(), 1);

            if(it == TmpCol.end());///this we want not
            else
            {
                m++;///add a row again...
                TmpMatrix.push_back(vector<int>(n, 0));
                TmpMatrix[m - 1][i] = -1;
            }
        }
        else if(cnt == m)///isol here
        {
            m++;
            TmpMatrix.push_back(vector<int>(n, 0));
            TmpMatrix[m - 1][i] = 1;
            m++;
            TmpMatrix.push_back(vector<int>(n, 0));
            TmpMatrix[m - 1][i] = -1;
        }
    }


///FILE SAVE///


    output.open("out.txt", ios::out | ios::trunc);///open in overwrite mode

    for(i = 0; i < n; i++)
    {
        TmpCol.clear();
        line.clear();
        line.insert(line.begin(), 3, '0');

        for(j = 0; j < m; j++) TmpCol.push_back(TmpMatrix[j][i]);
        auto it = find(TmpCol.begin(), TmpCol.end(), 4);
        if(it != TmpCol.end())
        {
            k = (distance(TmpCol.begin(), it)) + 1;
            output<<k;
            output<<'>';
            output<<k;
            output<<'\n';
        }
        else
        {
            it = find(TmpCol.begin(), TmpCol.end(), -1);
            k = (distance(TmpCol.begin(), it)) + 1;
            output<<k;
            output<<'>';
            it = find(TmpCol.begin(), TmpCol.end(), 1);
            k = (distance(TmpCol.begin(), it)) + 1;
            output<<k;
            output<<'\n';
        }
    }
    output.close();
/*
    cout<<endl;
    for(i = 0; i < n; i++) cout<<"\t"<<AdjVertList[i];

    for(i = 0; i < m; i++)///printing a proper incidence matrix(-1 is a beginning, 1 is end of a edge, 4 is self)
    {
        cout<<endl;
        cout<<i + 1<<"\t";
        for(j = 0; j < n; j++) cout<<TmpMatrix[i][j]<<"\t";
    }*/
}
