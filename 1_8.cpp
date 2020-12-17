/*
 * @Author       : Gehrychiang
 * @LastEditTime : 2020-12-17 18:42:51
 * @Website      : www.yilantingfeng.site
 * @E-mail       : gehrychiang@aliyun.com
 */
//0 存款 1取款
//#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int current;
int opentime;
char rev[2][10] = {"deposit", "withdraw"};
class bank_queue
{
private:
    typedef struct node
    {
        int t_in;
        int opr;
        int ct;
        int t_out;
        node *nxt;
    } qnode, *bk_queue;

    bk_queue head;

public:
    bank_queue()
    {
        head = new qnode;
        head->nxt = NULL;
    }
    ~bank_queue()
    {
        qnode *q;
        while (head)
        {
            q = head;
            head = head->nxt;
            delete q;
        }
    }

    void push(qnode q)
    {
        qnode *p = head;
        while (p->nxt)
        {
            p = p->nxt;
        }
        qnode *add = new qnode;
        add->ct = q.ct;
        add->nxt = NULL;
        add->opr = q.opr;
        add->t_in = q.t_in;
        add->t_out = q.t_out;
        p->nxt = add;
    }

    int insert()
    {
        qnode *q = new qnode;
        scanf("%d %d %d", &q->t_in, &q->opr, &q->ct);
        while (q->t_in > opentime || q->opr > 1 || q->opr < 0 || q->ct < 0)
        {
            printf("Invaild input! Please retry !\a\n");
            scanf("%d %d %d", &q->t_in, &q->opr, &q->ct);
        }
        printf("Yeah! we have got a customer who is going to enter the bank at %d second and he is going to %s %d yuan\n", q->t_in, rev[q->opr], q->ct);
        qnode *p = head;
        while (p->nxt)
        {
            p = p->nxt;
        }
        p->nxt = q;
        q->nxt = NULL;
        return q->t_in;
    }

    void pop()
    {
        if (head->nxt)
        {
            head->nxt = head->nxt->nxt;
        }
    }

    qnode *front()
    {
        if (head->nxt)
        {
            return head->nxt;
        }
        else
        {
            return NULL;
        }
    }

    bool empty()
    {
        if (head->nxt == NULL)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    int count()
    {
        int cnt = 0;
        qnode *p = head;
        while (p->nxt)
        {
            cnt++;
            p = p->nxt;
        }
        return cnt;
    }
};

void solve(bank_queue &fir, bank_queue &sec, bank_queue &res, int tm)
{
    while (!fir.empty())
    {
        if (fir.front()->opr == 0)
        {
            current += fir.front()->ct;
            fir.front()->t_out = tm;
            res.push(*fir.front());
            fir.pop();
        }
        else
        {
            if (fir.front()->ct <= current)
            {
                current -= fir.front()->ct;
                fir.front()->t_out = tm;
                res.push(*fir.front());
                fir.pop();
            }
            else
            {
                sec.push(*fir.front());
                fir.pop();
            }
        }
    }

    for (int i = 0; i < sec.count(); i++)
    {
        if (sec.front()->ct <= current)
        {
            current -= sec.front()->ct;
            sec.front()->t_out = tm;
            res.push(*sec.front());
            sec.pop();
        }
        else
        {
            sec.push(*sec.front());
            sec.pop();
        }
    }
}

int closedown(bank_queue &sec, bank_queue &res)
{
    int ans = 0;
    while (!sec.empty())
    {
        sec.front()->t_out = opentime;
        res.push(*sec.front());
        sec.pop();
    }
    int n = res.count();
    while (!res.empty())
    {
        ans += (res.front()->t_out - res.front()->t_in);
        res.pop();
    }
    return ans / n;
}
int main()
{
    //freopen("","r",stdin);
    //freopen("","w",stdout);
    opentime = 28800;
    printf("Please input the customer's infos.....\n");
    printf("The format is like \"{time} {operation} {money}\"\n");
    printf("such as input\n840 1 3200\nmeans a customer enter the bank at the 840 second and he want to withdraw 3200 yuan");
    printf("\n\nnow please enter the number of the customers:");
    int n;
    scanf("%d", &n);
    bank_queue fir;
    bank_queue sec;
    bank_queue res;
    printf("Good! Now please input the customer's infos.....(Ascending by time only)\n");
    for (int i = 0; i < n; i++)
    {
        int tm = fir.insert();
        solve(fir, sec, res, tm);
    }
    printf("\nThe average waiting time is %ds\n", closedown(sec, res));
    system("pause");
    return 0;
}