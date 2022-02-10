//========================
//ע���ѹ����ʱ��Ŀ���ļ���Ӧ�ô��� ����������Ŀ�겻��һ���ļ��е���ʾ 
//ע�ⴴ���ļ�ʱӦ�ü�ס��� !!KEY!! ,��������ʺͲ������ļ��ı�Ҫǰ�ᣡ(�û����������д) 
//https://blog.csdn.net/qq_45245039/article/details/116424777���ͻ�ûд�꣡ 
//ʹ��pac -h �鿴����˵����ף����Ŀ��ģ� 
//==========================
#define ONE_OF_KEY 42           //42-*

#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

class Head {
private:
    char Key[64];
    char creatorName[16];       //����������
    char version[16];           //�汾��
    char lastWriter[16];        //����޸���
//    time_t 	time_create;        //�ļ�����ʱ��
//    time_t 	time_last_write;    //�ļ�����޸�ʱ��
    int count;                  //ѹ���ļ����ܸ���(������û������)
    long long rear;             //ָ��β�ڵ�
public:
    Head(){};
    Head(const char *name, const char *ver,const char *k = nullptr);
    void writeHead2pac(char * pacFileName);//��ͷд�ظ����ļ�
    friend ostream &operator<<(ostream &os, const Head &head);//����<<�����
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
    struct _finddata_t fi;                //��ѹ���ļ��Ļ�����Ϣ
    long long pos;                            //��ѹ���ļ���ѹ�����Ŀ�ͷ
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
    int item_long;             //��һ���item����
    //Item *item;
    long long next;        //��һ��item��ĵ�ַ

public:
    ItemManger(int len = INDEX_LONG) {
        item_long = len;
        next = -1;
    }

    void showAllItems(ifstream &ifs);   //�ݹ��ӡ����itemֱ���ļ�β
    void unPacAllItems(ifstream &ifs, const char *pathTo);//�ݹ��ѹ����item��pathToֱ���ļ�β

    static void addItems(char *pacFileName) {//���INDEX_LONG��item
        Item item;
        ItemManger itemManger;
        ofstream ofs(pacFileName, ios::binary | ios::app);
        ofs.write((char *) &itemManger.item_long, sizeof(itemManger.item_long));
        for (int i = 0; i < itemManger.item_long; i++) ofs.write((char *) &item, sizeof(Item));
        ofs.write((char *) &itemManger.next, sizeof(itemManger.next));
        ofs.close();
    }

    static int these_isFull(ifstream &ifs) {//�жϵ�ǰָ���items���Ƿ��Ѿ����ˣ����˷���-1�����򷵻�����item����
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
    if (!ifs.is_open()) {//����ļ�������
        ifs.close();
        cout << "There is no file: " << pacFileName << " !!" << endl;
        cout << "And then, I will create " << pacFileName << " !" << endl;
        login();//��д��������Ϣ
        createPackFile();//����ͷ�ļ�
    } else {//�Ѿ������ļ�
        ifs.close();
        loadHeadInPacFile();//��ȡ�ļ�ͷ����Ϣ
        cout<< *head << endl;
        login();//��д�޸������ֺ��ļ���������
        if (!checkKey()) {//����������룬����ƥ��
            cout << "Error! your KEY isn't match this file!" << endl
                 << "you can ask this file_creator: " << head->getCreatorName()
                 << " for help!" << endl;
            exit(0);
        }
        //����ƥ��
        cout << "Hello! " << userName << endl;
    }
}

void Pac::login() {
    memset(myKey, ONE_OF_KEY, sizeof(myKey));
    cout << "Please input your name:";
    cin >> userName;
    cout << "Please input file_KEY:";
    cin >> myKey; //�����뻹û�����
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
    //����޸��� ��ʱ������
    head->setLastWriter(userName);
    //head->setLastTime(time)
    head->writeHead2pac(pacFileName);
}

void Pac::pushFileItem(const char *path, Item &item) {
    int full = noNeedItems();
    if (full == -1) {//β�ڵ�����
        fstream fs(pacFileName, ios::binary | ios::in | ios::out);
        fs.seekg(0, ios::end);
        long long end = fs.tellg();//��¼�ļ��½��ڵ����ʼλ��
        fs.seekg(head->getRear(), ios::beg);
        int size;
        fs.read((char *) &size, sizeof(size));//��ȡ�ļ�β�ڵ��item��
        fs.seekg(size * sizeof(Item), ios::cur);
        fs.write((char *) &end, sizeof(end));//�����ļ��е�ǰβ�ڵ㣬ָ���½ڵ���ʼ
        fs.close();
        head->setRear(end);//�����ڴ���ͷ����βָ��
        ItemManger::addItems(pacFileName);//����½ڵ�Ŀ�item��
        fs.open(pacFileName, ios::binary | ios::in | ios::out);
        fs.seekg(0, ios::end);
        long long pos = fs.tellg();//��¼�����item��pos
        item.setPos(pos);
        fs.seekg(end + sizeof(size), ios::beg);
        fs.write((char *) &item, sizeof(item));//��item��Ϣд���ļ�
        fs.close();
    } else {//β�ڵ�û��
        fstream fs(pacFileName, ios::binary | ios::in | ios::out);
        fs.seekg(0, ios::end);
        long long pos = fs.tellg();//��¼�����item��pos
        item.setPos(pos);
        fs.seekg(head->getRear() + sizeof(int) + full * sizeof(Item), ios::beg);//����ƹ�����item
        fs.write((char *) &item, sizeof(item));
        fs.close();
    }
    head->addCount();
    updateHead();//д���µ�ͷ��
    pushFileBuf(path, item);//д���ļ�������Ϣ
}


int Pac::noNeedItems() {
    if (head->getCount() == 0) return -1;//û�ļ���Ҫ��item��
    ifstream ifs(pacFileName, ios::binary | ios::in);
    ifs.seekg(head->getRear(), ios::beg);//������һ��Item��
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
    ifs.read(fbuf, item.getFi().size);//ע��ڶ���������sizeof�÷���
    ifs.close();

    ofstream ofs(pacFileName, ios::binary | ios::app | ios::out);
    ofs.write(fbuf, item.getFi().size);
    delete fbuf;
    ofs.close();
}

int Pac::pick2Pack(const char *path, const char *ext) {
    long hnd;
    Item pm;

    //���ζ�ȡ��ѹ���ļ�
    char fns[_MAX_FNAME];
    sprintf(fns, "%s\\%s", path, ext);

    hnd = _findfirst(fns, &pm.getFi());
    if (hnd < 0) { return -1; }

    if ((pm.getFi().attrib & _A_SUBDIR) != _A_SUBDIR) {
        cout << "add:"<<endl<<pm << endl;//<<����
        pushFileItem(path, pm);
    }
    while (!_findnext(hnd, &pm.getFi())) {
        if ((pm.getFi().attrib & _A_SUBDIR) == _A_SUBDIR) continue;
        cout << pm << endl;
        pushFileItem(path, pm);          //��ȡ�ļ�
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

int Pac::unPackFile(const char *pathTo) {//�Ǻ�catpacFolderһ��˼·д�ģ�����֮������һ��
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
}//����·���ϲ����ڵ��ļ����ļ��к����жϵ������ļ����ļ���(BUG)

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
        if (string::npos != is_file){//���ļ�
            int nPos = path.find_last_of('\\');
            if(string::npos != nPos){//�ļ�·��+�ļ���
                ext = path.substr(nPos+1);
                path = path.substr(0,nPos);
            }else{//ֻ���ļ���
                ext = path;
                path = ".";
            }
        }
    //    cout<<path<<"  "<<ext<<endl;
        pac.pick2Pack(path.c_str(),ext.c_str());
    }
    else if (!strcmp(argv[1], "-free2f")) {
        if (!fileORfold(argv[3])) {//���ļ���
            pac.unPackFile(argv[3]);//argv[3]�ļ��в�����ʱ������(BUG)
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

