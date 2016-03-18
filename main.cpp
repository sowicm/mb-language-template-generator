
/*
todo : renmae lord_talk main_ops
todo : insert lord_talk:sdf.14|xasfxvasdf

todo : dialogs_english.csv
todo : dialogs_todo.csv  (把这个拖动到上面，则生成dialogs_done.csv)
todo : dialogs.csv
todo : reg match {s0}{s1}{s2} 这样的全部不要显示（包括中间的空格）
todo : 重复的一模一样的不要显示（只有空格差别的也不要显示）
todo : 如果todo.csv 和原来的一模一样的就不要加进done.csv里面

*/

#include <Windows.h>
#include <stdio.h>
#include <string>
#include <set>
#include <algorithm>
#include <regex>
using std::set;
using std::string;
using std::move;
using std::sort;
using std::regex;
using std::regex_match;

class nodet
{
public:
    nodet()       {}

    nodet(nodet&& t)
    {
        id = t.id;
        hasTrans = t.hasTrans;
        hasValidTrans = t.hasValidTrans;
        a = move(t.a);
        b = move(t.b);
        c = move(t.c);
        d = move(t.d);
    }
    bool operator == (const nodet& t) const
    {
        return (a == t.a && b == t.b);
    }
    bool operator < (const nodet& t) const
    {
        if (a < t.a || (a == t.a && b < t.b))
            return true;
        return false;
    }
    int id;
    bool hasTrans;
    bool hasValidTrans;
    string a, b, c, d;
};
class nodet2
{
public:
    //nodet2(){}
    nodet2(nodet&& t)
    {
        id = t.id;
        hasTrans = t.hasTrans;
        hasValidTrans = t.hasValidTrans;
        a = move(t.a);
        b = move(t.b);
        c = move(t.c);
        d = move(t.d);
    }
    bool operator == (const nodet2& t) const
    {
        return id == t.id;
    }
    bool operator < (const nodet2& t) const
    {
        return id < t.id;
    }
    int id;
    bool hasTrans;
    bool hasValidTrans;
    string a, b, c, d;
};

void clearfiles()
{
    DeleteFile("dialogs_english.csv");
    DeleteFile("dialogs_unused.csv");
    DeleteFile("dialogs_redundancy.csv");
}

int idcmp(const nodet& a, const nodet& b)
{
    return a.id - b.id;
}

int APIENTRY WinMain(HINSTANCE hInst,
                     HINSTANCE hPrevInst,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    bool isDone = false;
    
    if (*lpCmdLine == '"')
    {
        ++lpCmdLine;
        *(lpCmdLine + strlen( lpCmdLine ) - 1) = 0;
    }
    string path = lpCmdLine;
    int index = path.find_first_of("dialogs_todo.csv");
    if (index >= 0)
    {
        isDone = true;
    }

    if (!isDone)
        clearfiles();

    char buf[10240];

    FILE *fp;
    char *pstart, *pend;
    set<nodet> dialogs;
    nodet t;
    char ch;
    int id = 0;
    fp = fopen("conversation.txt", "rb");
    while (fgets(buf, 10240, fp))
    {
        if (strncmp(buf, "dlga_", 5) != 0)
            continue;

        pend = pstart = buf;
        while (*++pend)
            if (*pend == ':')
                break;
        if (!*pend)
            continue;
        t.a = move(string(pstart, pend));
        pstart = pend + 1;
        while (*++pend)
            if (*pend == ' ')
                break;
        if (!*pend)
            continue;
        t.b = move(string(pstart, pend));
        if (dialogs.find(t) != dialogs.end())
        {
            //log(string("conversation.txt中找到重复项：") + t.a + ":" + t.b);
            continue;
        }
        pstart = pend;
        t.c = "";
        while (*++pstart)
        {
            ch = *pstart;
            if (ch == ' ' || ch == '-' || '0' <= ch && ch <= '9')
                continue;
            pend = pstart;
            while (*++pend)
                if (*pend == ' ')
                    break;
            if (!*pend)
                break;
            t.c = move(string(pstart, pend));
            break;
        }
        replace(t.c.begin(), t.c.end(), '_', ' ');
        t.c += "\r\n";
        t.id = ++id;
        t.hasTrans = false;
        t.hasValidTrans = false;
        dialogs.insert( move(t) );
    }
    fclose(fp);
    fp = fopen("dialogs.csv", "rb");
    int utf8header;
    fread(&utf8header, 3, 1, fp);
    while (fgets(buf, 10240, fp))
    {
        if (strncmp(buf, "dlga_", 5) != 0)
            continue;

        pend = pstart = buf;
        while (*++pend)
            if (*pend == ':')
                break;
        if (!*pend)
            continue;
        t.a = move(string(pstart, pend));
        pstart = pend + 1;
        while (*++pend)
            if (*pend == '|')
                break;
        if (!*pend)
            continue;
        t.b = move(string(pstart, pend));
        pstart = pend;

        auto itr = dialogs.find(t);
        if (itr == dialogs.end())
        {
            if (!isDone)
            {
                //log(string("dialogs.csv中找到多余的项（不会包含进dialogs_todo.csv，但会保存到dialogs_unused.csv）：") + t.a + ":" + t.b);
                FILE *fpu = fopen("dialogs_unused.csv", "ab");
                fputs(buf, fpu);
                fclose(fpu);
            }
            continue;
        }
        if (itr->hasTrans)
        {
            if (!isDone)
            {
                //log(string("dialogs.csv中找到重复的项（不会包含进dialogs_todo.csv， 但会保存到dialogs_redundancy.csv）:") + t.a + ":" + t.b);
                FILE *fpu = fopen("dialogs_redundancy.csv", "ab");
                fputs(buf, fpu);
                fclose(fpu);
            }
            continue;
        }
        pstart = pend + 1;
#if 0
        while (*++pend)
            if (*pend == '\n')
                break;
        if (!*pend)
            continue;
#endif
        t.id = itr->id;
        t.c = move(((nodet)*itr).c);
        t.d = pstart;
        t.hasTrans = true;
        t.hasValidTrans = (!t.d.empty() && t.c != t.d);
        dialogs.erase(itr);
        dialogs.insert( move(t) );
    }
    fclose(fp);

    if (isDone)
    {
        fp = fopen(path.c_str(), "rb");
        fread(&utf8header, 3, 1, fp);
        while (fgets(buf, 10240, fp))
        {
            if (strncmp(buf, "dlga_", 5) != 0)
                continue;

            pend = pstart = buf;
            while (*++pend)
                if (*pend == ':')
                    break;
            if (!*pend)
                continue;
            t.a = move(string(pstart, pend));
            pstart = pend + 1;
            while (*++pend)
                if (*pend == '|')
                    break;
            if (!*pend)
                continue;
            t.b = move(string(pstart, pend));
            pstart = pend;

            auto itr = dialogs.find(t);
            if (itr == dialogs.end())
            {
                continue;
            }
            pstart = pend + 1;
            t.id = itr->id;
            t.c = move(((nodet)*itr).c);
            t.d = pstart;
            t.hasTrans = true;
            t.hasValidTrans = (!t.d.empty() && t.c != t.d);
            dialogs.erase(itr);
            dialogs.insert( move(t) );
        }
        fclose(fp);
    }

    set<nodet2> res;
    for (auto itr = dialogs.begin(); itr != dialogs.end(); ++itr)
    {
        res.insert(move((nodet)*itr));
    }

    if (!isDone)
    {
        
        fp = fopen("dialogs_todo.csv", "wb");
        FILE *fp_more = fopen("dialogs_todo_more.csv", "wb");
        FILE *fp_english = fopen("dialogs_english.csv", "wb");
        //int utf8header = 0xBFBBEF;//0xEFBBBF;
        fwrite(&utf8header, 3, 1, fp);
        string s;
        for (auto itr = res.begin(); itr != res.end(); ++itr)
        {
            s = itr->a + ":" + itr->b + "|" + itr->c;
            fwrite(s.data(), 1, s.length(), fp_english);

            if (!itr->hasValidTrans)
            {
                if (!regex_match(itr->c, regex("([ ]*\\{s[0-9]+\\}[ ]*)+\\r\\n")))
                {
                    s = itr->a + ":" + itr->b + "|" + itr->c;
                    fwrite(s.data(), 1, s.length(),
                           (itr->c[0] == '{' && itr->c[1] == '!' && itr->c[2] == '}') ? fp_more : fp
                          );
                }
            }
        }
        fclose(fp);
        fclose(fp_more);
        fclose(fp_english);
        MessageBox(0, "生成了模板文件dialogs_todo.csv", "完成", 0);
    }
    else
    {
        fp = fopen("dialogs_done.csv", "wb");
        fwrite(&utf8header, 3, 1, fp);
        string s;
        for (auto itr = res.begin(); itr != res.end(); ++itr)
        {
            if (itr->hasValidTrans)
            {
                s = itr->a + ":" + itr->b + "|" + itr->d;
                fwrite(s.data(), 1, s.length(), fp);
            }
        }
        fclose(fp);
        MessageBox(0, "最终文件已经生成： dialogs_done.csv", "完成", 0);
    }
}