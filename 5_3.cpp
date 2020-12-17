/*
 * @Author       : Gehrychiang
 * @LastEditTime : 2020-12-17 18:53:29
 * @Website      : www.yilantingfeng.site
 * @E-mail       : gehrychiang@aliyun.com
 */
//#pragma GCC optimize(2)
#include <bits/stdc++.h>
#include <io.h>
using namespace std;
typedef long long ll;
template <typename key_elem, typename dt_elem>
class hash_map //用户给出关键字完成检索
{
private:
    typedef struct
    {
        int mod;
        int base;
        int n;
        int size;
    } info;

    typedef struct
    {
        key_elem key; //关键字
        dt_elem data;
        int next;
    } FLNode;

    info hash_basic;
    FLNode *hash_key, *hash_stg;
    fstream fp_info, fp_key, fp_stg;

    void hash_init(int par) //初始化 //扩展按两倍扩展
    {
        //cout << "debug" << "\n";
        if (par == 1)
        {
            fp_info.open("hash_info.bin", ios::in | ios::binary);
            fp_key.open("hash_key.bin", ios::in);
            fp_stg.open("hash_stg.data", ios::in);

            fp_info.read((char *)&hash_basic, sizeof(info));
            hash_key = (FLNode *)calloc(hash_basic.mod, sizeof(FLNode));
            for (int i = 0; i < hash_basic.mod; i++)
            {
                hash_key[i].next = -1; //置空
            }
            for (int i = 0; i < hash_basic.mod; i++)
            {
                fp_key >> hash_key[i].next;
            }
            hash_stg = (FLNode *)calloc(hash_basic.size + 10, sizeof(FLNode));
            for (int i = 0; i < hash_basic.size; i++)
            {
                hash_stg[i].next = -1; //置空
            }
            for (int i = 0; i < hash_basic.n; i++)
            {
                key_elem tmpa;
                dt_elem tmpb;
                int c;
                fp_stg >> tmpa >> tmpb >> c;
                hash_stg[i].key = tmpa;
                hash_stg[i].data = tmpb;
                hash_stg[i].next = c;
                //fp_stg >> hash_stg[i].key >> hash_stg[i].data >> hash_stg[i].next;
            }
        }
        else
        {
            hash_basic.mod = 2729;
            hash_basic.base = 131; //实际个数
            hash_basic.n = 0;      //实际个数
            hash_basic.size = 1;   //数组大小
            hash_key = (FLNode *)calloc(hash_basic.mod, sizeof(FLNode));
            for (int i = 0; i < hash_basic.mod; i++)
            {
                hash_key[i].next = -1; //置空
            }
            hash_stg = (FLNode *)calloc(hash_basic.size, sizeof(FLNode));
            for (int i = 0; i < hash_basic.size; i++)
            {
                hash_stg[i].next = -1; //置空
            }
        }
    }
    //哈希函数自立实现三个
    int hash_cal(int key)
    {
        //ret key_hashed
        int ret = 0;
        while (key)
        {
            ret = (ret * hash_basic.base + key % 10) % hash_basic.mod;
            key /= 10;
        }
        return ret % hash_basic.mod;
    }
    int hash_cal(string key)
    {
        //ret key_hashed
        int ret = 0;
        int len = key.length();
        for (int i = 0; i < len; i++)
        {
            ret = (ret * hash_basic.base + key[i]) % hash_basic.mod;
        }
        return ret % hash_basic.mod;
    }
    int hash_cal(char *key)
    {
        //ret key_hashed
        int ret = 0;
        int len = strlen(key);
        for (int i = 0; i < len; i++)
        {
            ret = (ret * hash_basic.base + key[i]) % hash_basic.mod;
        }
        return ret % hash_basic.mod;
    }

    bool hash_uni(key_elem key, dt_elem data)
    {
        //cout << hash_cal(key) << "\n";
        int ter = hash_key[hash_cal(key)].next;
        while (ter != -1)
        {
            if (hash_stg[ter].key == key)
            {
                hash_stg[ter].data = data;
                return true;
            }
            ter = hash_stg[ter].next;
        }
        return false;
    }

    void hash_insert(key_elem key, dt_elem data)
    {
        //cout << hash_cal(key) << "\n";
        if (hash_uni(key, data))
        {
            cout << "Already existed!(Auto modified)"
                 << "\n";
            return;
        }
        if (hash_key[hash_cal(key)].next == -1) //无后
        {
            hash_key[hash_cal(key)].next = hash_basic.n;
            if (hash_basic.n >= hash_basic.size) //扩容
            {
                hash_basic.size *= 2;
                FLNode *hash_stg_new = (FLNode *)calloc(hash_basic.size, sizeof(FLNode));
                for (int i = 0; i < hash_basic.size / 2; i++)
                {
                    *(hash_stg_new + i) = *(hash_stg + i);
                }
                for (int i = hash_basic.size / 2; i < hash_basic.size; i++)
                {
                    hash_stg_new[i].next = -1; //置空
                }
                free(hash_stg);
                hash_stg = hash_stg_new;
            }
            hash_stg[hash_basic.n].key = key;
            hash_stg[hash_basic.n].data = data;
            hash_basic.n++;
        }
        else
        {
            int ter = hash_key[hash_cal(key)].next;
            while (hash_stg[ter].next != -1)
            {
                ter = hash_stg[ter].next;
            }
            hash_stg[ter].next = hash_basic.n;
            if (hash_basic.n >= hash_basic.size) //扩容
            {
                hash_basic.size *= 2;
                FLNode *hash_stg_new = (FLNode *)calloc(hash_basic.size, sizeof(FLNode));
                for (int i = 0; i < hash_basic.size / 2; i++)
                {
                    *(hash_stg_new + i) = *(hash_stg + i);
                }
                for (int i = hash_basic.size / 2; i < hash_basic.size; i++)
                {
                    hash_stg_new[i].next = -1; //置空
                }
                free(hash_stg);
                hash_stg = hash_stg_new;
            }
            hash_stg[hash_basic.n].key = key;
            hash_stg[hash_basic.n].data = data;
            hash_basic.n++;
        }
    }

    dt_elem hash_ser(key_elem key)
    {
        //cout << hash_cal(key) << "\n";
        int ter = hash_key[hash_cal(key)].next;
        while (ter != -1)
        {
            if (hash_stg[ter].key == key)
            {
                return hash_stg[ter].data;
            }
            ter = hash_stg[ter].next;
        }
        dt_elem fault;
        cout << "Not Found(The result is only dirty value)";
        return fault;
    }

    void hash_erase(key_elem key)
    {
        int ter = hash_key[hash_cal(key)].next;
        if (ter == -1)
        {
            return;
        }
        else
        {
            if (hash_stg[ter].key == key) //第一个就是
            {
                hash_key[hash_cal(key)].next = hash_stg[ter].next;
            }
            else
            {
                while (hash_stg[ter].next != -1 && hash_stg[hash_stg[ter].next].key != key)
                {
                    ter = hash_stg[ter].next;
                }
                if (hash_stg[ter].next != -1 && hash_stg[hash_stg[ter].next].key == key)
                {
                    hash_stg[ter].next = hash_stg[hash_stg[ter].next].next;
                }
            }
        }
        // hash_basic.n--;
    }

public:
    hash_map()
    {
        //cout << "con" << "\n";
        //cout << fopen("hash_info.bin", "r") << "\n";
        if (!access("hash_info.bin", F_OK) && !access("hash_key.bin", F_OK) && !access("hash_stg.data", F_OK)) //文件存在
        {
            //fp_info("hash_info.bin");
            cout << "Previous data detected! Initilize or load from that?"
                 << "\n";
            cout << "(Enter 0 for reinitilize and 1 for load)"
                 << "\n";
            int opr;
            while (cin >> opr, opr != 0 && opr != 1)
            {
                cout << "Invaild input! Please Try Again!"
                     << "\n";
            }
            hash_init(opr);
        }
        else //不存在，写入
        {
            hash_init(0);
        }
    }
    ~hash_map()
    {
        //清空缓冲区
        fp_info.close();
        fp_key.close();
        fp_stg.close();

        fp_info.open("hash_info.bin", ios::out | ios::binary);
        fp_info.write((char *)&hash_basic, sizeof(info));
        fp_key.open("hash_key.bin", ios::out);
        fp_stg.open("hash_stg.data", ios::out);
        for (int i = 0; i < hash_basic.mod; i++)
        {
            fp_key << hash_key[i].next << "\n";
        }
        free(hash_key);

        for (int i = 0; i < hash_basic.n; i++)
        {
            fp_stg << hash_stg[i].key << " " << hash_stg[i].data << " " << hash_stg[i].next << "\n";
        }
        free(hash_stg);
        fp_info.close();
        fp_key.close();
        fp_stg.close();
    }

    void insert(key_elem key, dt_elem data)
    {
        hash_insert(key, data);
    }

    dt_elem find(key_elem key)
    {
        return hash_ser(key);
    }

    void erase(key_elem key)
    {
        hash_erase(key);
    }
};

int main()
{
    //freopen("","r",stdin);
    //freopen("","w",stdout);
    hash_map<string, string> hs;

    while (1)
    {
        cout << "\nWhat operation do you want to take\n1.insert a key_value\n2.search a value by a key\n3.delete a value by a key\n4.exit\n";
        int opr;
        while (cin >> opr, opr <= 0 || opr >= 5)
        {
        };
        string ina;
        string inb;
        switch (opr)
        {
        case 1:
            cout << "please enter a key and a value divided by a space" << endl;
            cin >> ina >> inb;
            hs.insert(ina, inb);
            cout << "done"
                 << "\n";
            break;
        case 2:
            cout << "please enter a key" << endl;
            cin >> ina;
            cout << "The value is " << hs.find(ina) << "\n";
            cout << "done"
                 << "\n";
            break;
        case 3:
            cout << "please enter a key" << endl;
            cin >> ina;
            hs.erase(ina);
            cout << "done"
                 << "\n";
            break;
        case 4:
            return 0;
            break;
        }
    }
    system("pause");
    return 0;
}