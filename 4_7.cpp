/*
 * @Author       : Gehrychiang
 * @LastEditTime : 2020-12-15 19:53:12
 * @Website      : www.yilantingfeng.site
 * @E-mail       : gehrychiang@aliyun.com
 */
//#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int maxm = 1e5 + 5;
const int maxn = 500 + 5;
typedef struct arcnode
{
    int adjvex;
    int cost;
    char typ;
    struct arcnode *nextarc;
} arcnode;

typedef struct vnode
{
    int data;
    arcnode *firstarc;
} vnode, adjlist[505];

typedef struct alg
{
    adjlist verts;
    int vexnum, arcnum;
} alg;

typedef struct dis
{
    int cp;
    int rt[maxn];
    char tp[maxn];
    bool operator<(const dis &b) const //bigint小于比较
    {
        int ret = 0;
        int retb = 0;
        int rt_pre[maxn];
        int rt_preb[maxn];
        for (int i = 0; i < cp; i++)
        {
            if (i == 0)
            {
                rt_pre[i] = rt[i];
            }
            else
            {
                rt_pre[i] = rt_pre[i - 1] + rt[i];
            }
        }
        for (int i = 0; i < b.cp; i++)
        {
            if (i == 0)
            {
                rt_preb[i] = b.rt[i];
            }
            else
            {
                rt_preb[i] = rt_preb[i - 1] + b.rt[i];
            }
        }
        for (int i = 0, j = 0; i < cp; i++)
        {
            while (j + 1 < cp && tp[j + 1] == tp[i])
            {
                j++;
            }
            //[i,j]闭区间
            if (tp[i] == 'n')
            {
                if (i == 0)
                {
                    ret = ret + (rt_pre[j]) * (rt_pre[j]);
                }
                else
                {
                    ret = ret + (rt_pre[j] - rt_pre[i - 1]) * (rt_pre[j] - rt_pre[i - 1]);
                }
            }
            else
            {
                if (i == 0)
                {
                    ret = ret + (rt_pre[j]);
                }
                else
                {
                    ret = ret + (rt_pre[j] - rt_pre[i - 1]);
                }
            }
            i = j;
        }
        for (int i = 0, j = 0; i < b.cp; i++)
        {
            while (j + 1 < b.cp && b.tp[j + 1] == b.tp[i])
            {
                j++;
            }
            //[i,j]闭区间
            if (b.tp[i] == 'n')
            {
                retb = retb + (rt_preb[j] - rt_preb[i - 1]) * (rt_preb[j] - rt_preb[i - 1]);
            }
            else
            {
                retb = retb + (rt_preb[j] - rt_preb[i - 1]);
            }
            i = j;
        }
        return ret < retb;
    }
} dis;

priority_queue<pair<int, int>> q;

ll rt_cost(dis a)
{
    ll ret = 0;
    ll rt_pre[maxn];
    for (int i = 0; i < a.cp; i++)
    {
        if (i == 0)
        {
            rt_pre[i] = a.rt[i];
        }
        else
        {
            rt_pre[i] = rt_pre[i - 1] + a.rt[i];
        }
    }
    for (int i = 0, j = 0; i < a.cp; i++)
    {
        while (j + 1 < a.cp && a.tp[j + 1] == a.tp[i])
        {
            j++;
        }
        //[i,j]闭区间
        if (a.tp[i] == 'n')
        {
            if (i == 0)
            {
                ret = ret + (rt_pre[j]) * (rt_pre[j]);
            }
            else
            {
                ret = ret + (rt_pre[j] - rt_pre[i - 1]) * (rt_pre[j] - rt_pre[i - 1]);
            }
        }
        else
        {
            if (i == 0)
            {
                ret = ret + (rt_pre[j]);
            }
            else
            {
                ret = ret + (rt_pre[j] - rt_pre[i - 1]);
            }
        }
        i = j;
    }
    return ret;
}

void cre_udg(alg &g)
{
    cin >> g.vexnum >> g.arcnum;
    for (int i = 1; i <= g.vexnum; i++) //init
    {
        g.verts[i].data = i;
        g.verts[i].firstarc = NULL;
    }
    for (int k = 0; k < g.arcnum; ++k) //cre
    {
        int typ, v1, v2, w;
        char opr;
        cin >> typ >> v1 >> v2 >> w;
        if (typ)
        {
            opr = 'n';
        }
        else
        {
            opr = 'b';
        }
        arcnode *p1 = new arcnode;
        p1->adjvex = v2;
        p1->cost = w;
        p1->typ = opr;
        p1->nextarc = g.verts[v1].firstarc;
        g.verts[v1].firstarc = p1;

        arcnode *p2 = new arcnode;
        p2->adjvex = v1;
        p2->cost = w;
        p2->typ = opr;
        p2->nextarc = g.verts[v2].firstarc;
        g.verts[v2].firstarc = p2;
    }
}

void dji(alg g)
{
    int ans[maxn];
    dis tpr[maxn];
    int vis[maxn];
    for (int i = 0; i < maxn; i++)
    {
        ans[i] = 1e7;
    }
    memset(tpr, 0, sizeof(tpr));
    memset(vis, 0, sizeof(vis));
    ans[1] = 0; //源点
    q.push(make_pair(0, 1));
    while (!q.empty())
    {
        int u = q.top().second; //当前点
        q.pop();
        if (!vis[u])
        {
            vis[u] = true;
            for (arcnode *i = g.verts[u].firstarc; i; i = i->nextarc) //从head向后做遍历直到0
            {
                int v = i->adjvex;
                int w = i->cost;
                dis tmp = tpr[u];
                tmp.rt[tmp.cp] = w;
                tmp.tp[tmp.cp] = i->typ;
                tmp.cp++;
                if (rt_cost(tmp) < ans[v])
                {
                    tpr[v] = tmp;
                    ans[v] = rt_cost(tmp);
                    q.push(make_pair(-ans[v], v));
                }
            }
        }
    }
    printf("%d\n", ans[g.vexnum]);
}

int main()
{
    //freopen("","r",stdin);
    //freopen("","w",stdout);
    alg g;
    cre_udg(g);
    dji(g);
    return 0;
}