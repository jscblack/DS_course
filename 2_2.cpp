/*
 * @Author       : Gehrychiang
 * @LastEditTime : 2020-12-17 18:40:50
 * @Website      : www.yilantingfeng.site
 * @E-mail       : gehrychiang@aliyun.com
 */
//#pragma GCC optimize(2)
#include <windows.h>
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
void SetColor(unsigned short color)
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon, color);
}
template <typename Elemtype>
class m_queue
{
private:
    typedef struct QNode
    {
        Elemtype data;
        QNode *next;
    } QNode, *Queue;

    Queue head;

public:
    m_queue()
    {
        head = new QNode;
        head->next = NULL;
    }
    ~m_queue()
    {
        QNode *q;
        while (head)
        {
            q = head;
            head = head->next;
            delete q;
        }
    }

    void push(Elemtype in)
    {
        QNode *add = new QNode;
        QNode *p = head;
        while (p->next)
        {
            p = p->next;
        }
        add->next = NULL;
        add->data = in;
        p->next = add;
    }

    Elemtype front()
    {
        return head->next->data;
    }

    void pop()
    {
        if (head->next)
        {
            QNode *p = head->next;
            head->next = head->next->next;
            delete p;
        }
    }

    bool empty()
    {
        if (head->next == NULL)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};
struct node
{
    int x;
    int y;
    int step;
    string ans;
};
m_queue<node> q;
string bfs(int n, int m, char **maze)
{
    int sx[] = {1, 0, 0, -1};
    int sy[] = {0, -1, 1, 0};
    char ss[] = {'D', 'L', 'R', 'U'};
    bool **vis = (bool **)calloc(sizeof(bool *), n);
    for (int i = 0; i < n; i++)
    {
        *(vis + i) = (bool *)calloc(sizeof(bool), m);
    }
    string out;
    int max_step = 0x7fffffff;
    int stx = 0, sty = 0;
    vis[0][0] = true;
    q.push({stx, sty, 0, ""});
    while (!q.empty())
    {
        node tmp = q.front();
        q.pop();
        if (tmp.x == n - 1 && tmp.y == m - 1)
        {
            if (max_step >= tmp.step)
            {
                max_step = tmp.step;
                out = tmp.ans;
            }
        }
        else
        {
            for (int i = 0; i < 4; i++)
            {
                if (0 <= tmp.x + sx[i] && tmp.x + sx[i] <= n - 1 && 0 <= tmp.y + sy[i] && tmp.y + sy[i] <= m - 1 && !vis[tmp.x + sx[i]][tmp.y + sy[i]] && maze[tmp.x + sx[i]][tmp.y + sy[i]] == '0')
                {
                    q.push({tmp.x + sx[i], tmp.y + sy[i], tmp.step + 1, tmp.ans + ss[i]});
                    vis[tmp.x + sx[i]][tmp.y + sy[i]] = true;
                }
            }
        }
    }
    return out;
}
int main()
{
    //freopen("", "r", stdin);
    //freopen("", "w", stdout);
    printf("Now please enter the number of the column and row of the maze!\n");
    int n, m;
    scanf("%d %d", &n, &m);
    char **maze = (char **)calloc(sizeof(char *), n);
    printf("Please enter a matrix with %d rows and %d columns\n(0 means spare which allows to pass through while 1 means a obstcle)\n", n, m);
    for (int i = 0; i < n; i++)
    {
        *(maze + i) = (char *)calloc(sizeof(char), m);
        scanf("%s", *(maze + i));
    }
    string ans = bfs(n, m, maze);
    cout << "\nThe best route is: " << ans << "\n\n";
    int p = 0;
    int psx = 0, psy = 0;
    do
    {
        if (ans[p] == 'D' || ans[p] == 'U')
        {
            maze[psx][psy] = ans[p];
            psx += ((ans[p] == 'D') ? 1 : -1);
        }
        else
        {
            maze[psx][psy] = ans[p];
            psy += (ans[p] == 'R' ? 1 : -1);
        }
        p++;
    } while (p <= ans.length());

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (i == n - 1 && j == m - 1)
            {
                SetColor(240);
                cout << "O";
                break;
            }
            switch (maze[i][j])
            {
            case 'U':
                SetColor(240);
                cout << "¡ü";
                break;
            case 'D':
                SetColor(240);
                cout << "¡ý";
                break;
            case 'L':
                SetColor(240);
                cout << "¡û";
                break;
            case 'R':
                SetColor(240);
                cout << "¡ú";
                break;
            default:
                SetColor(15);
                cout << maze[i][j];
                break;
            }
        }
        cout << endl;
    }
    SetColor(15);
    system("pause");
    return 0;
}
/*
01010101001011001001010110010110100100001000101010
00001000100000101010010000100000001001100110100101
01111011010010001000001101001011100011000000010000
01000000001010100011010000101000001010101011001011
00011111000000101000010010100010100000101100000000
11001000110101000010101100011010011010101011110111
00011011010101001001001010000001000101001110000000
10100000101000100110101010111110011000010000111010
00111000001010100001100010000001000101001100001001
11000110100001110010001001010101010101010001101000
00010000100100000101001010101110100010101010000101
11100100101001001000010000010101010100100100010100
00000010000000101011001111010001100000101010100011
10101010011100001000011000010110011110110100001000
10101010100001101010100101000010100000111011101001
10000000101100010000101100101101001011100000000100
10101001000000010100100001000100000100011110101001
00101001010101101001010100011010101101110000110101
11001010000100001100000010100101000001000111000010
00001000110000110101101000000100101001001000011101
10100101000101000000001110110010110101101010100001
00101000010000110101010000100010001001000100010101
10100001000110010001000010101001010101011111010010
00000100101000000110010100101001000001000000000010
11010000001001110111001001000011101001011011101000
00000110100010001000100000001000011101000000110011
10101000101000100010001111100010101001010000001000
10000010100101001010110000000100101010001011101000
00111100001000010000000110111000000001000000001011
10000001100111010111010001000110111010101101111000

DDDDRRURRRRRRDRRRRDDDLDDRDDDDDDDDDDDDRDDRRRURRUURRDDDDRDRRRRRRDRRURRDDDRRRRUURUUUUUUULULLUUUURRRRUULLLUUUULLUUULUURRURRURURRRDDRRRRRDDRRDDLLLDDRRDDRDDLDDDLLDDLLLDLDDDLDDRRRRRRRRRDDDDDDRR

*/