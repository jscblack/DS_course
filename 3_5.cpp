/*
 * @Author       : Gehrychiang
 * @LastEditTime : 2020-12-17 18:47:57
 * @Website      : www.yilantingfeng.site
 * @E-mail       : gehrychiang@aliyun.com
 */
//#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
template <typename Elemtype>
class AVL_tree
{
private:
    typedef struct
    {
        int key;
        Elemtype data;
    } elm;
    typedef struct avlnode
    {
        elm data;
        int h;
        struct avlnode *lchild, *rchild;
    } avlnode, *avltree;

    avltree root; //超根节点 并非具有实际意义 其左节点指向当前的根节点

    void avl_intrav(avltree t)
    {
        if (t)
        {
            avl_intrav(t->lchild);
            cout << t->data.key << " ";
            avl_intrav(t->rchild);
        }
    }

    int avl_height(avlnode *t)
    {
        if (t)
        {
            return t->h;
        }
        else
        {
            return -1;
        }
    }

    void SingleRotateWithLeft(avltree &t)
    {
        avlnode *p = t->lchild;
        t->lchild = p->rchild;
        p->rchild = t;

        t->h = max(avl_height(t->lchild), avl_height(t->rchild)) + 1;
        p->h = max(avl_height(t->lchild), t->h) + 1;
        t = p;
    }

    void SingleRotateWithRight(avltree &t)
    {
        avlnode *p = t->rchild;
        t->rchild = p->lchild;
        p->lchild = t;
        t->h = max(avl_height(t->lchild), avl_height(t->rchild)) + 1;
        p->h = max(avl_height(t->rchild), t->h) + 1;
        t = p;
    }

    void DoubleRotateWithLeft(avltree &t)
    {
        SingleRotateWithRight(t->lchild);
        SingleRotateWithLeft(t);
    }

    void DoubleRotateWithRight(avltree &t)
    {
        SingleRotateWithLeft(t->rchild);
        SingleRotateWithRight(t);
    }

    void avl_insert(avltree &t, int key, Elemtype data)
    {
        if (!t)
        {
            avltree add = new avlnode;
            add->data.key = key;
            add->data.data = data;
            add->lchild = add->rchild = NULL;
            add->h = max(avl_height(add->lchild), avl_height(add->rchild)) + 1;
            t = add;
        }
        else if (key < t->data.key)
        {
            avl_insert(t->lchild, key, data);
            if (avl_height(t->lchild) - avl_height(t->rchild) == 2)
            {
                if (key < t->lchild->data.key)
                {
                    SingleRotateWithLeft(t);
                }
                else
                {
                    DoubleRotateWithLeft(t);
                }
            }
            t->h = max(avl_height(t->lchild), avl_height(t->rchild)) + 1;
        }
        else if (t->data.key < key)
        {
            avl_insert(t->rchild, key, data);
            if (avl_height(t->lchild) - avl_height(t->rchild) == -2)
            {
                if (key > t->rchild->data.key)
                    SingleRotateWithRight(t);
                else
                {
                    DoubleRotateWithRight(t);
                }
            }
            t->h = max(avl_height(t->lchild), avl_height(t->rchild)) + 1;
        }
    }

    avltree avl_ser(avltree t, int key) //用于重载返回地址
    {
        if (!t || t->data.key == key)
        {
            return t;
        }
        else if (t->data.key > key)
        {
            return avl_ser(t->lchild, key);
        }
        else
        {
            return avl_ser(t->rchild, key);
        }
    }

    void avl_erase(avltree &t, int key)
    {
        if (!t)
        {
            return;
        }
        else if (key < t->data.key)
        {
            avl_erase(t->lchild, key);
            if (avl_height(t->lchild) - avl_height(t->rchild) == -2)
            {
                if (avl_height(t->lchild) - avl_height(t->rchild->rchild) == -1)
                    SingleRotateWithRight(t);
                else
                    DoubleRotateWithRight(t);
            }
            t->h = max(avl_height(t->lchild), avl_height(t->rchild)) + 1;
        }
        else if (key > t->data.key)
        {
            avl_erase(t->rchild, key);
            if (avl_height(t->lchild) - avl_height(t->rchild) == 2)
            {
                if (avl_height(t->lchild->lchild) - avl_height(t->rchild) == 1)
                    SingleRotateWithLeft(t);
                else
                    DoubleRotateWithLeft(t);
            }
            t->h = max(avl_height(t->lchild), avl_height(t->rchild)) + 1;
        }
        else
        {
            if (t->lchild == NULL && t->rchild == NULL)
            {
                delete t;
                t = NULL;
            }
            else if (t->lchild != NULL && t->rchild == NULL)
            {
                avlnode *p = t;
                t = t->lchild;
                delete p;
            }
            else if (t->lchild == NULL && t->rchild != NULL)
            {
                avlnode *p = t;
                t = t->rchild;
                delete p;
            }
            else
            {
                avlnode *p = t->rchild;
                while (p->lchild != NULL)
                {
                    p = p->lchild;
                }
                t->data = p->data;
                if (p == t->rchild) //特殊情况
                    t->rchild = p->rchild;
                else
                { //一般情况
                    t->rchild->lchild = p->rchild;
                    t->rchild->h = max(avl_height(t->rchild->lchild), avl_height(t->rchild->rchild)) + 1;
                }
                delete p;
            }
            if (t != NULL)
                t->h = max(avl_height(t->lchild), avl_height(t->rchild)) + 1;
        }
    }

public:
    AVL_tree()
    {
        root = new avlnode;
        root->lchild = NULL;
        root->rchild = NULL;
        root->h = 0;
    }
    ~AVL_tree() //待完善
    {
        delete root;
    }

    void insert(int key, Elemtype data)
    {
        avl_insert(root->lchild, key, data);
    }
    void intrav()
    {
        avl_intrav(root->lchild);
    }
    Elemtype find(int key)
    {
        avlnode *ret = avl_ser(root->lchild, key);
        if (ret)
        {
            cout << "Found"
                 << "\n";
            return ret->data.data;
        }
        else
        {
            Elemtype ret;
            cout << "Not Found(return dirty value)"
                 << "\n";
            return ret; //此时返回值不可信
        }
    }
    void erase(int key)
    {
        avl_erase(root->lchild, key);
    }
};

template <typename Elemtype>
class BS_tree
{
private:
    typedef struct
    {
        int key;
        Elemtype data;
    } elm;
    typedef struct bstnode
    {
        elm data;
        struct bstnode *lchild, *rchild;
    } bstnode, *bstree;

    bstree root; //超根节点 并非具有实际意义 其左节点指向当前的根节点
    void bst_insert(bstree &t, int in, Elemtype data)
    {
        if (!t)
        {
            bstree add = new bstnode;
            add->data.key = in;
            add->data.data = data;
            add->lchild = add->rchild = NULL;
            t = add;
        }
        else if (t->data.key > in)
        {
            bst_insert(t->lchild, in, data);
        }
        else if (t->data.key < in)
        {
            bst_insert(t->rchild, in, data);
        }
    }

    bstree bst_ser(bstree t, int tar) //用于重载返回地址
    {
        if (!t || t->data.key == tar)
        {
            return t;
        }
        else if (t->data.key > tar)
        {
            return bst_ser(t->lchild, tar);
        }
        else
        {
            return bst_ser(t->rchild, tar);
        }
    }

    void bst_intrav(bstree t)
    {
        if (t)
        {
            bst_intrav(t->lchild);
            cout << t->data.key << " ";
            bst_intrav(t->rchild);
        }
    }

    void bst_erase(bstree &t, int key)
    {
        bstree p = t, f = NULL, q;
        while (p)
        {
            if (p->data.key == key)
                break;
            f = p;
            if (p->data.key > key)
            {
                p = p->lchild;
            }
            else
            {
                p = p->rchild;
            }
        }
        if (!p)
        {
            return;
        }
        else
        {
            q = p;
            if (p->lchild && p->rchild)
            {
                bstree tmp = p->lchild;
                while (tmp->rchild)
                {
                    q = tmp;
                    tmp = tmp->rchild;
                }
                p->data = tmp->data;
                if (q != p)
                {
                    q->rchild = tmp->lchild;
                }
                else
                {
                    q->lchild = tmp->lchild;
                }
                delete tmp;
                return;
            }
            else if (p->lchild)
            {
                p = p->lchild;
            }
            else if (p->rchild)
            {
                p = p->rchild;
            }
        }
        if (!f)
        {
            t = p;
        }
        else if (q == f->lchild)
        {
            f->lchild = p;
        }
        else
        {
            f->rchild = p;
        }
        delete q;
    }

public:
    BS_tree()
    {
        root = new bstnode;
        root->lchild = NULL;
        root->rchild = NULL;
    }
    ~BS_tree()
    {
        delete root;
    }

    void insert(int key, Elemtype data)
    {
        bst_insert(root->lchild, key, data);
    }

    Elemtype find(int key)
    {
        bstnode *ret = bst_ser(root->lchild, key);
        if (ret)
        {
            cout << "Found"
                 << "\n";
            return ret->data.data;
        }
        else
        {
            Elemtype ret;
            cout << "Not Found(return dirty value)"
                 << "\n";
            return ret; //此时返回值不可信
        }
    }

    void intrav()
    {
        bst_intrav(root->lchild);
    }

    void erase(int key)
    {
        bst_erase(root->lchild, key);
    }
};

int main()
{
    //freopen("","r",stdin);
    //freopen("","w",stdout);
    while (1)
    {
    start:
        printf("Which one do you want to build\n1.avl_tree\n2.bs_tree\n3.exit\n");
        int type;
        while (scanf("%d", &type), type != 1 && type != 2 && type != 3)
        {
            printf("Invaild input! Please retry!\n");
        }
        if (type == 1)
        {
            AVL_tree<int> t;
            printf("Built!\n");
            int opr;
            while (1)
            {
                printf("\nWhat operation do you want to act\n1.insert a number\n2.delete a number\n3.print the tree\n4.find one key and print the data\n5.exit\n");
                while (scanf("%d", &type), type != 1 && type != 2 && type != 3 && type != 4 && type != 5)
                {
                    printf("Invaild input! Please retry!\n");
                }
                int src;
                switch (type)
                {
                case 1:
                    printf("Please enter the number you want to insert\n");
                    scanf("%d", &src);
                    t.insert(src, src);
                    printf("Finished\n");
                    break;
                case 2:
                    printf("Please enter the number you want to delete\n");
                    scanf("%d", &src);
                    t.erase(src);
                    printf("Finished\n");
                    break;
                case 3:
                    printf("The tree is\n");
                    t.intrav();
                    printf("\n");
                    break;
                case (4):
                    printf("Please enter the key you want to find\n");
                    scanf("%d", &src);
                    cout << "The value is " << t.find(src) << endl;
                    break;
                case (5):
                    goto start;
                }
            }
        }
        else if (type == 2)
        {
            BS_tree<int> t;
            printf("Built!\n");
            int opr;
            while (1)
            {
                printf("\nWhat operation do you want to act\n1.insert a number\n2.delete a number\n3.print the tree\n4.find one key and print the data\n5.exit\n");
                while (scanf("%d", &type), type != 1 && type != 2 && type != 3 && type != 4 && type != 5)
                {
                    printf("Invaild input! Please retry!\n");
                }
                int src;
                switch (type)
                {
                case 1:
                    printf("Please enter the number you want to insert\n");
                    scanf("%d", &src);
                    t.insert(src, src);
                    printf("Finished\n");
                    break;
                case 2:
                    printf("Please enter the number you want to delete\n");
                    scanf("%d", &src);
                    t.erase(src);
                    printf("Finished\n");
                    break;
                case 3:
                    printf("The tree is\n");
                    t.intrav();
                    printf("\n");
                    break;
                case (4):
                    printf("Please enter the key you want to find\n");
                    scanf("%d", &src);
                    cout << "The value is " << t.find(src) << endl;
                    break;
                case (5):
                    goto start;
                }
            }
        }
        else
        {
            break;
        }
    }
    system("pause");
    return 0;
}