//========================
//注意解压操作时，目标文件夹应该存在 ，否则会出现目标不是一个文件夹的提示 
//注意创建文件时应该记住你的 !!KEY!! ,这是你访问和操作你文件的必要前提！(用户名可以随便写) 
//https://blog.csdn.net/qq_45245039/article/details/116424777博客还没写完！ 
//使用pac -h 查看命令说明，祝你玩的开心！ 
//==========================
#define ONE_OF_KEY 42           //42-*

#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

class Head {
private:
    char Key[64];
    char creatorName[16];       //创建者名字
    char version[16];           //版本号
    char lastWriter[16];        //最后修改人
//    time_t 	time_create;        //文件创建时间
//    time_t 	time_last_write;    //文件最后修改时间
    int count;                  //压缩文件的总个数(对链表没有意义)
    long long rear;             //指向尾节点
public:
    Head(){};
    Head(const char *name, const char *ver,const char *k = nullptr);
    void writeHead2pac(char * pacFileName);//将头写回更新文件
    friend ostream &operator<<(ostream &os, const Head &head);//重载<<运算符
    const char *getKey() const;
    const char *getCreatorName() const;
    const int getCount() const;
    const long long getRear() const;
    void setLastWriter(const char* laWriter);
    void setLastTime(const time_t lt);
    void setVersion(const char* ver);
    void setRear(long long int rear);
    void addCount();
};

Head::Head(const char *name, const char *ver, const char *k) {
    memset(creatorName, 0, sizeof(creatorName));
    strcpy(creatorName, name);
    memset(version, 0, sizeof(version));
    strcpy(version, ver);
    memset(lastWriter, 0, sizeof(lastWriter));
    strcpy(lastWriter, name);
    memset(Key, ONE_OF_KEY, sizeof(Key));
    if (k != nullptr && strcmp(k, "")) strcpy(Key, k);
    count = 0;
    rear = sizeof(Head);
}

void Head::writeHead2pac(char *pacFileName) {
    fstream fs(pacFileName, ios::binary | ios::out | ios::in);
    fs.write((char *) this, sizeof(Head));
    fs.close();
}

ostream &operator<<(ostream &os, const Head &head) {
    os << "************ This pacFile ************" << endl
       << "\tCreatorName:\t" << head.creatorName << endl
       << "\tversion:\t" << head.version << endl
       << "\tlastWriter:\t" << head.lastWriter << endl
       << "\tcount:\t\t" << head.count << endl
       << "**************************************";
    return os;
}

const int Head::getCount() const {
    return count;
}

const char *Head::getKey() const {
    return Key;
}

void Head::addCount() {
    this->count++;
}

const char *Head::getCreatorName() const {
    return creatorName;
}

const long long Head::getRear() const {
    return rear;
}

void Head::setLastWriter(const char *laWriter) {
    strcpy(lastWriter, laWriter);
}

void Head::setVersion(const char *ver) {
    strcpy(version, ver);
}

void Head::setLastTime(time_t) {

}

void Head::setRear(long long int rear) {
    this->rear = rear;
}
#include<fstream>
#include<sys/stat.h>
#include <ctime>
#include <iostream>

class Item {
private:
    struct _finddata_t fi;                //被压缩文件的基本信息
    long long pos;                            //被压缩文件在压缩包的开头
    friend class ItemMenger;

public:
    Item() : pos(-1) { memset(&fi, 0, sizeof(fi)); }

    const char *sec2dt(time_t sec) const {
        static char buf[64];
        struct tm *t = localtime(&sec);
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", t);
        return buf;
    }

    const char *getCreateTime() const { return sec2dt(fi.time_create); }

    _finddata64i32_t &getFi() { return fi; }

    long long &getPos() { return pos; }

    void setPos(long long int pos) {
        this->pos = pos;
    }

    friend ostream &operator<<(ostream &os, Item item) {
        os << item.getFi().name << "\t\t"
           << item.getFi().size << "\t\t"
           << item.getCreateTime();
        return os;
    }
};

#define INDEX_LONG 3


class ItemManger {
private:
    int item_long;             //这一块的item个数
    //Item *item;
    long long next;        //下一块item块的地址

public:
    ItemManger(int len = INDEX_LONG) {
        item_long = len;
        next = -1;
    }

    void showAllItems(ifstream &ifs);   //递归打印所有item直到文件尾
    void unPacAllItems(ifstream &ifs, const char *pathTo);//递归解压所有item到pathTo直到文件尾

    static void addItems(char *pacFileName) {//填充INDEX_LONG个item
        Item item;
        ItemManger itemManger;
        ofstream ofs(pacFileName, ios::binary | ios::app);
        ofs.write((char *) &itemManger.item_long, sizeof(itemManger.item_long));
        for (int i = 0; i < itemManger.item_long; i++) ofs.write((char *) &item, sizeof(Item));
        ofs.write((char *) &itemManger.next, sizeof(itemManger.next));
        ofs.close();
    }

    static int these_isFull(ifstream &ifs) {//判断当前指向的items块是否已经满了，满了返回-1，否则返回已有item个数
        int size;
        ifs.read((char *) &size, sizeof(size));
        Item item;
        for (int i = 0; i < size; i++) {
            ifs.read((char *) &item, sizeof(item));
            if (item.getPos() == -1) return i;
        }
        return -1;
    }
};

void ItemManger::showAllItems(ifstream &ifs) {
    ifs.read((char *) &item_long, sizeof(item_long));
    static Item item;
    for (int i = 0; i < item_long; i++) {
        ifs.read((char *) &item, sizeof(item));
        if (item.getPos() == -1) return;
        cout << item << endl;
    }
    cout << ">->->->->->->->->->->->->->->->->->->" << endl;
    ifs.read((char *) &next, sizeof(next));
    if (next > 0) {
        ifs.seekg(next, ios::beg);
        showAllItems(ifs);
    }
}

void ItemManger::unPacAllItems(ifstream &ifs, const char *pathTo) {
    ifs.read((char *) &item_long, sizeof(item_long));
    static Item item;
    for (int i = 0; i < item_long; i++) {
        ifs.read((char *) &item, sizeof(item));
        if (item.getPos() == -1) return;
        long long now_where = ifs.tellg();
        ifs.seekg(item.getPos(), ios::beg);
        char *buf = new char[item.getFi().size];
        ifs.read(buf, item.getFi().size);
        char fnb[256];
        sprintf(fnb, "%s\\%s", pathTo, item.getFi().name);
        ofstream ofs(fnb, ios::binary | ios::out);
        ofs.write(buf, item.getFi().size);
        ofs.close();
        delete buf;
        cout << "unpac: " << item << endl;
        ifs.seekg(now_where, ios::beg);
    }
    ifs.read((char *) &next, sizeof(next));
    if (next > 0) {
        ifs.seekg(next, ios::beg);
        unPacAllItems(ifs, pathTo);
    }
}
class Pac {
private:
    char pacFileName[64];
    char userName[16];
    Head *head;
    //ItemManger *itemManger;

    char myKey[64];

    void login();

    void createPackFile();

    void loadHeadInPacFile();

    bool checkKey();

    void updateHead();

    int noNeedItems();

    void pushFileBuf(const char *path, Item &item);

public:
    Pac(char *FileName);

    ~Pac() { delete head; }


    void catPacFolder();

    int pick2Pack(const char *path, const char *ext);

    void pushFileItem(const char *path, Item &item);

    int unPackFile(const char *pathTo);


    void static showHelp() {
        cout << "hello world! welcome to my C++ code program! you can put in:" << endl;
        cout << "pac -cat\t(result.dat)\t\t\t\tto cat information of ALL FILE" << endl;
        //cout << "pac -geti (result.dat) (int index)\tto get number index FILE form .dat" << endl;
        cout << "pac -f2zip\t(result.dat) (Folder f or file)\t\tto zip ALL or (*.*) FILE of f" << endl;
        cout << "pac -free2f\t(result.dat) (Folder f)\t\t\tto free ALL FILE to f" << endl;
        cout << "wish there is NO BUG!" << endl;
    }
};

Pac::Pac(char *filename) {
    strcpy(pacFileName, filename);
    ifstream ifs(pacFileName, ios::binary | ios::in);
    if (!ifs.is_open()) {//如果文件不存在
        ifs.close();
        cout << "There is no file: " << pacFileName << " !!" << endl;
        cout << "And then, I will create " << pacFileName << " !" << endl;
        login();//填写创建者信息
        createPackFile();//创建头文件
    } else {//已经存在文件
        ifs.close();
        loadHeadInPacFile();//读取文件头部信息
        cout<< *head << endl;
        login();//填写修改这名字和文件访问密码
        if (!checkKey()) {//检验访问密码，若不匹配
            cout << "Error! your KEY isn't match this file!" << endl
                 << "you can ask this file_creator: " << head->getCreatorName()
                 << " for help!" << endl;
            exit(0);
        }
        //密码匹配
        cout << "Hello! " << userName << endl;
    }
}

void Pac::login() {
    memset(myKey, ONE_OF_KEY, sizeof(myKey));
    cout << "Please input your name:";
    cin >> userName;
    cout << "Please input file_KEY:";
    cin >> myKey; //空密码还没处理好
}

void Pac::createPackFile() {
    head = new Head(userName, "V1.0", myKey);
    ofstream ofs(pacFileName, ios::binary | ios::out);
    ofs.write((char *) head, sizeof(Head));
    ofs.close();
}

void Pac::loadHeadInPacFile() {
    head = new Head();
    ifstream ifs(pacFileName, ios::binary | ios::in);
    ifs.read((char *) head, sizeof(Head));
    ifs.close();
}

bool Pac::checkKey() {
    if (strcmp(myKey, head->getKey())) return false;
    return true;
}

void Pac::updateHead() {
    //最后修改人 和时间设置
    head->setLastWriter(userName);
    //head->setLastTime(time)
    head->writeHead2pac(pacFileName);
}

void Pac::pushFileItem(const char *path, Item &item) {
    int full = noNeedItems();
    if (full == -1) {//尾节点已满
        fstream fs(pacFileName, ios::binary | ios::in | ios::out);
        fs.seekg(0, ios::end);
        long long end = fs.tellg();//记录文件新建节点的起始位置
        fs.seekg(head->getRear(), ios::beg);
        int size;
        fs.read((char *) &size, sizeof(size));//读取文件尾节点的item数
        fs.seekg(size * sizeof(Item), ios::cur);
        fs.write((char *) &end, sizeof(end));//更改文件中当前尾节点，指向新节点起始
        fs.close();
        head->setRear(end);//更新内存中头部的尾指针
        ItemManger::addItems(pacFileName);//填充新节点的空item块
        fs.open(pacFileName, ios::binary | ios::in | ios::out);
        fs.seekg(0, ios::end);
        long long pos = fs.tellg();//记录结点中item的pos
        item.setPos(pos);
        fs.seekg(end + sizeof(size), ios::beg);
        fs.write((char *) &item, sizeof(item));//将item信息写入文件
        fs.close();
    } else {//尾节点没满
        fstream fs(pacFileName, ios::binary | ios::in | ios::out);
        fs.seekg(0, ios::end);
        long long pos = fs.tellg();//记录结点中item的pos
        item.setPos(pos);
        fs.seekg(head->getRear() + sizeof(int) + full * sizeof(Item), ios::beg);//向后移过满的item
        fs.write((char *) &item, sizeof(item));
        fs.close();
    }
    head->addCount();
    updateHead();//写入新的头部
    pushFileBuf(path, item);//写入文件内容信息
}


int Pac::noNeedItems() {
    if (head->getCount() == 0) return -1;//没文件，要填item块
    ifstream ifs(pacFileName, ios::binary | ios::in);
    ifs.seekg(head->getRear(), ios::beg);//检查最后一个Item块
    int f = ItemManger::these_isFull(ifs);
    ifs.close();
    return f;
}

void Pac::pushFileBuf(const char *path, Item &item) {
    char fn[_MAX_FNAME];
    char *fbuf;
    sprintf(fn, "%s\\%s", path, item.getFi().name);
    fbuf = new char[item.getFi().size];
    memset(fbuf, 0, item.getFi().size);

    ifstream ifs(fn, ios::binary | ios::in);
    ifs.read(fbuf, item.getFi().size);//注意第二个参数（sizeof用法）
    ifs.close();

    ofstream ofs(pacFileName, ios::binary | ios::app | ios::out);
    ofs.write(fbuf, item.getFi().size);
    delete fbuf;
    ofs.close();
}

int Pac::pick2Pack(const char *path, const char *ext) {
    long hnd;
    Item pm;

    //依次读取待压缩文件
    char fns[_MAX_FNAME];
    sprintf(fns, "%s\\%s", path, ext);

    hnd = _findfirst(fns, &pm.getFi());
    if (hnd < 0) { return -1; }

    if ((pm.getFi().attrib & _A_SUBDIR) != _A_SUBDIR) {
        cout << "add:"<<endl<<pm << endl;//<<重载
        pushFileItem(path, pm);
    }
    while (!_findnext(hnd, &pm.getFi())) {
        if ((pm.getFi().attrib & _A_SUBDIR) == _A_SUBDIR) continue;
        cout << pm << endl;
        pushFileItem(path, pm);          //读取文件
    }
    _findclose(hnd);
    return 0;
}

void Pac::catPacFolder() {
    ifstream ifs(pacFileName, ios::binary | ios::in);
    ifs.seekg(sizeof(Head), ios::beg);
    ItemManger it;
    cout << "-------------------------------------" << endl
         << "File name\tFile size\tCreate time" << endl;
    it.showAllItems(ifs);
    cout << "-------------------------------------" << endl;
    ifs.close();
}

int Pac::unPackFile(const char *pathTo) {//是和catpacFolder一个思路写的，可以之后整合一下
    ifstream ifs(pacFileName, ios::binary | ios::in);
    ifs.seekg(sizeof(Head), ios::beg);
    ItemManger it;
    it.unPacAllItems(ifs, pathTo);
    ifs.close();
    return 0;
}

bool fileORfold(char * const path){//true->file false->fold
    struct _stat buf;
    int result = _stat(path, &buf);
    //if (_S_IFREG & buf.st_mode) return true;
    if (_S_IFDIR & buf.st_mode) return false;
    else return true;
}//对于路径上不存在的文件和文件夹很难判断到底是文件和文件夹(BUG)

int main(int argc,char *argv[]){
    //for(int i = 0;i<argc;i++) cout<<argv[i]<<endl;
    if(argc<3) {
        if (argc == 2 && !strcmp(argv[1], "-h")) Pac::showHelp();
        else cout << "error command" << endl;
        return 0;
    }
    else if(!fileORfold(argv[2])){
        cout<<"error command"<<endl
            <<argv[2]<<" isn't a file"<<endl;
        return 0;
    }

    Pac pac = Pac(argv[2]);
    if (!strcmp(argv[1], "-cat")) pac.catPacFolder();
    else if (!strcmp(argv[1], "-f2zip")) {
        string path = argv[3];
        string ext = "*.*";
        int is_file = path.find_last_of('.');
        if (string::npos != is_file){//是文件
            int nPos = path.find_last_of('\\');
            if(string::npos != nPos){//文件路径+文件名
                ext = path.substr(nPos+1);
                path = path.substr(0,nPos);
            }else{//只有文件名
                ext = path;
                path = ".";
            }
        }
    //    cout<<path<<"  "<<ext<<endl;
        pac.pick2Pack(path.c_str(),ext.c_str());
    }
    else if (!strcmp(argv[1], "-free2f")) {
        if (!fileORfold(argv[3])) {//是文件夹
            pac.unPackFile(argv[3]);//argv[3]文件夹不存在时有问题(BUG)
        }else{
            cout<<"error command"<<endl
                <<argv[3]<<" isn't a fold"<<endl;
        }
    }
    else {
		cout << "error command" << endl;
		Pac::showHelp();
	}
    return 0;
}

