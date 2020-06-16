#include<algorithm>
//accumulate
#include<numeric>
#include<iostream>
#include<vector>
#include<list>
#include<array>
#include<string>
using namespace std;
//容器的泛型算法


//只读算法
void readOnlyAlthm()
{
    vector<int> l1 = {1,2,3,4,5};

    //查找，返回迭代器指针,未找到返回end
    auto res1 = find(l1.begin(), l1.end(), 4);
    auto res2 = find(l1.begin(), l1.end(), 10);
    //迭代器相减获取元素个数（好像list的迭代器不能相减？）
    cout << "location for 4: " << res1 - l1.begin() << endl;
    cout << "find 4: " << ((res1 == l1.end()) ? 0 : 1) << endl;
    cout << "find 10: " << ((res2 == l1.end()) ? 0 : 1) << endl;

    //泛型，同样适合数组
    int ia[] = {1,2,3,4,5};
    auto res3 = find(begin(ia), end(ia), 3);
    cout << "ia find 3: " << ((res1 == l1.end()) ? 0 : 1) << endl;

    //计数，返回出现的次数
    string s1 = "hello world!";
    int cnt = count(s1.begin(), s1.end(), 'l');
    cout << "l cnt: " << cnt << endl;

    vector<string> vec1 = {"hello", " ", "world", " !"};
    //累加，返回累加结果（累加操作取决于对象：字符串凭借、数字求和）
    string sum1 = accumulate(vec1.cbegin(), vec1.cend(), string(""));
    cout << sum1 << endl;

    //相等，判断序列元素是否都相等(前提：ia的长度>=l1的长度)，返回bool
    bool flag = equal(l1.begin(), l1.end(), begin(ia));
    cout << "equal :" << flag << endl;


}

//写容器算法
void writeAlthm()
{
    //填充
    vector<int> vec1 = {1,2,3};
    //范围写入相同的数(0,0,3),不能超范围
    fill(vec1.begin(), vec1.begin()+2, 0);
    cout << "fill: "<< vec1[0] << vec1[1] << vec1[2] << endl; 

    //指定个数写入,写入2个(1,1,3)
    fill_n(vec1.begin(), 2, 1);
    cout << "filln: "<< vec1[0] << vec1[1] << vec1[2] << endl; 

    //错误：不能超出容器大小
    // vector<int> vec2;
    // fill_n(vec2.begin(), 2, 0);

    //使用插入迭代器来解决上面的问题
    //等价于push_back,在容器后再插入三个0
    fill_n(back_inserter(vec1), 3, 0);
    cout << "back_insert: " << vec1.size() << endl;

    //拷贝
    string s1(10, '!');
    cout << "s1: "<< s1 << endl;
    string s2 = "hello word!";
    //s2子序列拷贝到s1中，返回s2写入的尾元素之后的位置
    //依然要注意s2的容量要多余写入的内容
    auto end = copy(s2.begin()+6, s2.end(), s1.begin());
    //继续尾元素后的位置写入
    copy(s2.begin(), s2.begin()+5, end);
    cout << "new s1: "<< s1 << endl;

    
    //替换
    //源序列直接替换
    replace(s2.begin(), s2.end(), 'l', 'L');
    cout << "repace: "<< s2 << endl;

    //拷贝替换
    string s3(20, '!');
    replace_copy(s2.begin(), s2.end(), s3.begin(), 'L', 'r');
    cout << "s3: " << s3 << endl;

}

//重排容器(以删除字母重复为例)
void elimDups()
{
    string s = "hello world";

    //重排使得相同字母在一起
    sort(s.begin(), s.end());
    cout << "sort: " << s << endl;
    //unique重排，相当于将第一个不重复的字母保留，后面的删除前移
    //因此重排结果是删除了三个重复元素，前移后，露出了最后的三个字母:orw
    //end_unique指向orw的第一个元素
    auto end_unique = unique(s.begin(), s.end());
    cout << "unique: " << s << endl;
    //泛型算法并不能改变容器大小，所以删除元素只能用容器的操作
    s.erase(end_unique, s.end());
    cout << "erase: " << s << endl;

}

int main()
{
    
    readOnlyAlthm();
    // writeAlthm();
    // elimDups();


}