#include "additionalwindow.h"
#include "confirmdelete.h"
#include "createchoice.h"
#include "exception.h"
#include "linkedpathwindow.h"
#include "mainwindow.h"
#include "renamewindow.h"
#include "ui_mainwindow.h"

#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);  //настраивает пользовательский интерфейс для указанного виджета
    setWindowTitle("FileManager");  //установка имени главного окна
    try {
        if(!(model = new QFileSystemModel(this)))  // выделение памяти под указатель на объект этого класса
            throw BadAllocException("Memory allocation", "Model of QFileSystemModel was not created!");
    }
    catch(BadAllocException error)
    {
        error.GetException(this);
    }
    catch (...)
    {
        QMessageBox::warning(this, "Memory allocation", "Unknown error! Please try again!");
    }
    model->setFilter(QDir::QDir::AllEntries); // метод позволяет отображать некоторые элементы файловой системы(в нашем случае все)
    model->setRootPath("/");                   // метод позволяет определить место в системе для отслеживания изменений(указана корневая папка)
    //система представлена в виде структурЫ типа List
    ui->lvSource->setModel(model);      //назначение *model объектом представления панели Source
    //устанока информации для подсказок
    ui->btnCreate->setToolTip("Create");
    ui->btnDelete->setToolTip("Delete");
    ui->btnCopy->setToolTip("Copy");
    ui->btnReplace->setToolTip("Replace");
    ui->btnRename->setToolTip("Rename");
    ui->lineSearch->setToolTip("Please don't forget about the extension, if you want to enter a file name");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}

void MainWindow::on_lvSource_clicked(const QModelIndex &index)
{
    //получение указателя на обЪект который принял сигнал
    //приведение объекта источника методом sender() к типу listview
    QFileInfo info = model->fileInfo(index); //получение пути элемента, который соответствует этому индексу
    // в зависимости от того, что это за элемент, алгоритм дальнейших дествий ветвится
    QString format = "dddd, d MMMM yy hh:mm:ss";    //формат вывода даты последнего изменения
    ui->lblDate->setText(info.lastModified().toString(format)); //вывод даты последнего изменения
    if(info.isFile())   //выбранный объект - файл
    {
        QString sizeFile=""; //переменная размера файла
        if(info.size()<1024)    //если размер файла меньше 1 килобайта
            sizeFile=sizeFile.append(QString::number(info.size())).append(" B");    //приведение размера к виду "B"
        else if(info.size()>=1024 && info.size()<1048576)    //если размер файла больше 1 килобайта и меньше 1 мегабайта
        {
            double d=info.size()/1024.;
            sizeFile=sizeFile.append(QString::number(d)).append(" KB"); //приведение размера к виду "KB"
        }
        else if(info.size()>=1048576 && info.size()<1073741824) //если размер файла больше 1 мегабайта и меньше 1 гигабайта
        {
            double d=info.size()/1048576.;
            sizeFile=sizeFile.append(QString::number(d)).append(" MB"); //приведение размера к виду "MB"
        }
        else if(info.size()>=1073741824)    //если размер файла больше 1 гигабайта
        {
            double d=info.size()/1073741824.;
            sizeFile=sizeFile.append(QString::number(d)).append(" GB"); //приведение размера к виду "GB"
        }
        QString size="Size : ";
        QString allSize=size.append(sizeFile);  //строка для отображения размера файла
        ui->lblSize->setText(allSize);  //отображениe размера файла
        QString type="Type : ";
        QString typeInfo=type.append(info.suffix()).append("-file");     //строка для отображения типа файла
        ui->lblType->setText(typeInfo); //отображениe типа файла
        fileName = model->fileName(index);  //переменная для хранения имени выбранного файла
        filePath = model->filePath(index);  //переменная для хранения пути выбранного файла
            //установка пути выбранного файла
        if(dirPath!="") //если до этого была выбрана директория
        {
            //очистка пути директории
            dirPath=dirName="";

        }
        if(linkPath!="")
        {
            linkPath=linkName="";

        }
    }
    else if(info.isDir()) //если выбранный объект - директория
    {
        ui->lblSize->setText("");  //отображение типа объекта
        ui->lblType->setText("Type: System directory");  //отображение типа объекта
        dirName=info.fileName();    //переменная для хранения имени выбранной директории
        dirPath=info.absoluteFilePath();    //переменная для хранения пути выбранной директории
        if(filePath!="")    //если до этого был выбран файл
        {
            //очистка пути файла
            filePath=fileName="";

        }
        if(linkPath!="")
        {
            linkPath=linkName="";

        }
    }
    else if(info.isSymLink())
    {
        QString sizelink=""; //переменная размера файла
        if(info.size()<1024)    //если размер файла меньше 1 килобайта
            sizelink=sizelink.append(QString::number(info.size())).append(" B");    //приведение размера к виду "B"
        else if(info.size()>=1024 && info.size()<1048576)    //если размер файла больше 1 килобайта и меньше 1 мегабайта
        {
            double d=info.size()/1024.;
            sizelink=sizelink.append(QString::number(d)).append(" KB"); //приведение размера к виду "KB"
        }
        else if(info.size()>=1048576 && info.size()<1073741824) //если размер файла больше 1 мегабайта и меньше 1 гигабайта
        {
            double d=info.size()/1048576.;
            sizelink=sizelink.append(QString::number(d)).append(" MB"); //приведение размера к виду "MB"
        }
        else if(info.size()>=1073741824)    //если размер файла больше 1 гигабайта
        {
            double d=info.size()/1073741824.;
            sizelink=sizelink.append(QString::number(d)).append(" GB"); //приведение размера к виду "GB"
        }
        QString size="Size : ";
        QString allSize=size.append(sizelink);  //строка для отображения размера файла
        ui->lblSize->setText(allSize);  //отображениe размера файла
        QString type="Type : ";
        QString typeInfo=type.append(info.suffix()).append("-link");     //строка для отображения типа файла
        ui->lblType->setText(typeInfo); //отображениe типа файла
        linkName = model->fileName(index);  //переменная для хранения имени выбранного файла
        linkPath = model->filePath(index);  //переменная для хранения пути выбранного файла
            //установка пути выбранного файла
        if(filePath!="")
        {
            filePath=fileName="";

        }
        if(dirPath!="") //если до этого была выбрана директория
        {
            //очистка пути директории
            dirPath=dirName="";

        }
    }
}

//слот обработки двойного клика мыши панели lvSource(переход к нажимаемому элементу)
void MainWindow::on_lvSource_doubleClicked(const QModelIndex &index)
{
    QListView* listView = (QListView*) sender();//получение указателя на обЪект который принял сигнал
    //приведение объекта источника методом sender() к типу listview
    //Возвращает указатель на объект, отправивший сигнал, если он вызван в слоте, активированном сигналом; в противном случае он возвращает nullptr.
    QFileInfo fileInfo = model->fileInfo(index); //получение информации элемента, который соответствует этому индексу
    // в зависимости от того, что это за элемент, алгоритм дальнейших дествий ветвится
    if(fileInfo.fileName()=="..")    //если выбран выход из текущей папки
    {
        QDir qDir = fileInfo.dir();//получение объекта класса QDir
        if(filePath!="") //если до этого был выбран файл
        {
            //очистка пути файла
            filePath=fileName="";

        }
        if(linkPath!="") //если до этого был выбран файл
        {
            //очистка пути файла
            linkPath=linkName="";

        }
        qDir.cd("..");//dir.cdUp();  навигация. в данном случае переход в родительскую папку
        listView->setRootIndex(model->index(qDir.absolutePath()));// получение индекса по пути
        //чтобы показать эелемент, полученный через индекс(listView->setRootIndex)
        //так работает списочное представление (в конкретный момент времени показано содержимое одной папки)
        ui->listWidget->clear(); //очистка панели "listWidget"
        ui->listWidget->addItem(fileInfo.absoluteFilePath());    //отображение нового пути в панели "listWidget"
        if(dirPath!="")  //если до этого выбрана директория
        {
            //очистка пути директории
            dirPath=dirName="";

        }
    }
    else if(fileInfo.fileName()==".")    //если выбран выход в корневую папку
    {
        if(filePath!="") //если до этого был выбран файл
        {
            //очистка пути файла
            filePath=fileName="";

        }
        if(linkPath!="") //если до этого был выбран файл
        {
            //очистка пути файла
            linkPath=linkName="";

        }
        listView->setRootIndex(model->index(""));//переход в корневую папку
        //показать корневую папку
        ui->listWidget->clear(); //очистка панели "listWidget"
        ui->listWidget->addItem(""); //отображение нового пути в панели "listWidget"
        if(dirPath!="")  //если до этого выбрана директория
        {
            //очистка пути директории
            dirPath=dirName="";


        }
    }
    else if(fileInfo.isDir())   // если выбранный элемент - директория
    {
        if(filePath!="") //если до этого был выбран файл
        {
            //очистка пути файла
            filePath=fileName="";

        }
        if(linkPath!="") //если до этого был выбран файл
        {
            //очистка пути файла
            linkPath=linkName="";

        }
        listView->setRootIndex(index);//элемент с этим индексом становится корневым
        ui->listWidget->clear(); //очистка панели "listWidget"
        ui->listWidget->addItem(fileInfo.absoluteFilePath());    //отображение нового пути в панели "listWidget"
            //очистка пути директории
        dirPath=dirName="";

    }
    else if(fileInfo.isFile())   // если выбранный элемент - файл
    {
        if(dirPath!="")  //если до этого выбрана директория
        {
            //очистка пути директории
            dirPath=dirName="";

        }
        if(linkPath!="") //если до этого был выбран файл
        {
            //очистка пути файла
            linkPath=linkName="";

        }
        //QDesktopServices::openUrl(QUrl::fromUserInput(filePath)); //открывает файл в файловой системе Windows
        //очистка пути файла
        filePath=fileName="";

    }
    else if(fileInfo.isSymLink())   // если выбранный элемент - файл
    {
        if(dirPath!="")  //если до этого выбрана директория
        {
            //очистка пути директории
            dirPath=dirName="";

        }
        if(filePath!="") //если до этого был выбран файл
        {
            //очистка пути файла
            filePath=fileName="";

        }
        //QDesktopServices::openUrl(QUrl::fromUserInput(filePath)); //открывает файл в файловой системе Windows
        //очистка пути файла
        linkPath=linkName="";

    }
}

void MainWindow::on_btnCreate_clicked() //слот нажатия на кнопку "Сreate"
{
    QDir qDir=QDir(model->filePath(ui->lvSource->rootIndex()));   //получение текущей директории
    try {
        if(qDir.absolutePath()=="/home/h4thqewjtch/qt/build-File-Desktop-Debug")   //если это корневая директория
            QMessageBox::warning(this,"Create", "You are in a root directory! Please choose an another directory");
        else
        {
            CreateChoice window;
            window.exec();  //метод выполняет появление окна для выбора типа создаваемого объекта
            if(window.GetFile()) //если был выбран файл
            {
                RenameWindow isFile;
                isFile.exec();  //выполняет появление окна для создания имени
                QString createPath = qDir.absolutePath().append("/").append(isFile.GetName()); //получение пути соозданного файла
                bool fileExists=false;  //флаг существования файлов с таким именем
                //цикл прохода по текущей директории для поиска файлов с таким именем
                foreach(QFileInfo files, qDir.entryInfoList(QDir::Files|QDir::NoDotAndDotDot, QDir::Name))
                {
                    if(files.fileName()==isFile.GetName())  //если файл с таким именем найден
                    {
                        fileExists=true; //флаг существования файла принимает истинное значение
                        break;
                    }
                }
                if(!fileExists) //если файлов с таким именем нет
                {
                        if(!file->Create(createPath)) //если файл не создан
                            throw PerformationException("Create File", "The operation <<Create>> was not perfomed!");
                }
                else throw PerformationException("Create File", "A file with this name exists!");

            }
            if(window.GetDir())  //если выбранный объект - директория0
            {
                RenameWindow isDir;
                isDir.exec();   //выполняет появление окна для создания имени
                QString createPath = qDir.absolutePath().append("/").append(isDir.GetName());    //получение пути сооздаваемой директории
                bool dirExists=false;//флаг существования файлов с таким именем
                //цикл прохода по текущей директории для поиска директории с таким именем
                foreach(QFileInfo dirs, qDir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot, QDir::Name))
                {
                    if(dirs.fileName()==isDir.GetName())    //если директория с таким именем найдена
                    {
                        dirExists=true; //флаг существования директории принимает истинное значение
                        break;
                    }
                }
                if(!dirExists)  //если директорий с таким именем нет
                {
                    if(!dir->Create(createPath))   //если директория не создана
                        throw  PerformationException( "Create Directory", "The operation <<Create>> was not perfomed!");
                }
                else throw  PerformationException( "Create Directory", "A directory with this name exist!");
            }
            if(window.GetLink())
            {
                RenameWindow isLink;
                isLink.exec();  //выполняет появление окна для создания имени
                QString createPath = qDir.absolutePath().append("/").append(isLink.GetName()); //получение пути соозданного файла
                LinkedPathWindow window;
                window.exec();  //выполняет появление окна для создания имени
                QString linkedPath = window.GetPath(); //получение пути соозданного файла
                bool linkExists=false;  //флаг существования файлов с таким именем
                //цикл прохода по текущей директории для поиска файлов с таким именем
                foreach(QFileInfo links, qDir.entryInfoList(QDir::Files|QDir::NoDotAndDotDot, QDir::Name))
                {
                    if(links.fileName()==isLink.GetName())  //если файл с таким именем найден
                    {
                        linkExists=true; //флаг существования файла принимает истинное значение
                        break;
                    }
                }
                if(!linkExists) //если файлов с таким именем нет
                {
                    QString path="";
                    if(!link->Create(path.append(linkedPath).append(" ").append(createPath))) //если файл не создан
                        throw PerformationException("Create Symbol Link", "The operation <<Create>> was not perfomed!");
                }
                else throw PerformationException("Create Symbol Link", "A symbol link with this name exists!");
            }
        }
    }

    catch(PerformationException error)
    {
        error.GetException(this);
    }
    catch (...)
    {
        QMessageBox::warning(this, "Create", "Unknown error! Please try again!");
    }
}

bool RecursiveDelete(QDir &qDir, System* file, System* dir, System *link)   //функция рекурсивного удаления содержимого выбранной папки
{
    //цикл прохода по текущей директории для удаления файлов и директорий внутри
    foreach(QFileInfo info, qDir.entryInfoList(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot, QDir::Name|QDir::DirsFirst))
    {
        if(info.isDir())//если директория
        {
            qDir.cd(info.fileName());//заходим в нее
            RecursiveDelete(qDir, file, dir, link);// рекурсивно удаляем внутренности
            //теперь папка пуста и мы можем ее удалить
            if(!dir->Delete(qDir.absolutePath()))
                return false;
            qDir.cdUp();//возврат
        }
        else if(info.isFile())  //если текущий объект - файл
        {
                if(!file->Delete(info.absoluteFilePath())) //если файл не удален
                    return false;
        }
        else if(info.isSymLink())  //если текущий объект - файл
        {
                if(!link->Delete(info.absoluteFilePath())) //если файл не удален
                    return false;
        }
    }
    return true;
}

void MainWindow::on_btnDelete_clicked() //слот нажатия на кнопку "Delete"
{
    QDir qDir=QDir(model->filePath(ui->lvSource->rootIndex()));  //получение текущей директории
    try {
        if(qDir.absolutePath()=="/home/h4thqewjtch/qt/build-File-Desktop-Debug")   //если это корневая директория
            QMessageBox::warning(this,"Delete", "You are in a root directory! Please choose an another directory");
        else
        {
            if(filePath=="" && dirPath=="" && linkPath=="") //если не выбран ни один объект
                throw ChoiseException( "Delete", "You was not choose a file or a directory! Please try again");
            else if(filePath!="" && dirPath=="" && linkPath=="")    //если выбран файл
            {
                ConfirmDelete window;
                window.exec();  //метод выполняет появление окна для подтверждения удаления
                if(!window.Confirm())  //если операция отменена
                    throw PerformationException( "Delete FIle", "The operation was canceled!");
                else
                {
                            //установка пути файла
                        if(!file->Delete(filePath)) //если удаление не выпонено
                            throw PerformationException( "Delete File", "The operation <<Delete>> was not perfomed!");
                        //очистка пути файла
                        filePath=fileName="";

                }
            }
            else if(filePath=="" && dirPath!="" && linkPath=="")     //если выбрана директория
            {
                ConfirmDelete window;
                window.exec();  //метод выполняет появление окна для подтверждения удаления
                if(!window.Confirm())   //если операция отменена
                    throw PerformationException( "Delete Directory", "The operation was canceled!");
                else
                {
                    QDir qDir=QDir(dirPath); //получение выбранной директории
                    if(!qDir.isEmpty())  //если директория не пуста
                    {
                        if(!RecursiveDelete(qDir, file, dir, link)) //если внутренние файлы не удалены
                            throw PerformationException( "Delete Directory", "The operation <<Delete>> was not perfomed!");
                    }
                    if(qDir.isEmpty())   //если директория пуста
                    {
                        //установка пути директории

                            if(!dir->Delete(dirPath))   //если удаление не выпонено
                            throw PerformationException( "Delete Directory", "The operation <<Delete>> was not perfomed!");
                        //очистка пути директории
                        dirPath=dirName="";

                    }
                }
            }
            else if(filePath=="" && dirPath=="" && linkPath!="")    //если выбран файл
            {
                ConfirmDelete window;
                window.exec();  //метод выполняет появление окна для подтверждения удаления
                if(!window.Confirm())  //если операция отменена
                    throw PerformationException( "Delete Symbol Link", "The operation was canceled!");
                else
                {
                            //установка пути файла
                        if(!link->Delete(linkPath)) //если удаление не выпонено
                            throw PerformationException( "Delete Symbol Link", "The operation <<Delete>> was not perfomed!");
                        //очистка пути файла
                        linkPath=linkName="";

                }
            }
        }
    }

    catch(ChoiseException error)
    {
        error.GetException(this);
    }
    catch(PerformationException error)
    {
        error.GetException(this);
    }
    catch(...)
    {
        QMessageBox::warning(this, "Delete", "Unknown error! Please try again!");
    }
}

void RecursiveCopyList(QDir &dir,QFileInfoList &copyList)   //функция рекурсивного наполнения содержимым списка для копирования
{
    //цикл прохода по текущей директории для создания контейнера с файлами и директориями внутри
    foreach(QFileInfo info, dir.entryInfoList(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot, QDir::Name|QDir::DirsFirst))
    {
        copyList.append(info);  //добавление элемента в контейнеp
        if(info.isDir()) // элемент - директория
        {
            dir.cd(info.fileName());//заходим в нее
            RecursiveCopyList(dir, copyList);// рекурсивно копируем содержимое
            dir.cdUp();//возврат
        }
    }
}

void MainWindow::on_btnCopy_clicked()   //слот нажатия на кнопку "Сopy"
{
    QDir qDir=QDir(dirPath); //получение выбранной директории
    try {
        if(fileName=="" && dirName=="" && linkName=="") //если не выбран ни один объект
            throw ChoiseException( "Copy", "You was not choose a file or a directory! Please try again");
        else if(fileName!="" && dirName=="" && linkName=="")    //если выбран файл
        {
            AdditionalWindow window;
            window.exec();  //метод выполняет появление дополнительного окна для копирования
            if(window.Cancel()) //если операция отменена
                throw PerformationException( "Copy File", "The operation was canceled!");
            else
            {
                QString copyPath = window.GetQDir().absolutePath().append("/").append(fileName);    //создание пути для копирования
                bool fileExists=false;  //флаг существования файла с таким именем
                //цикл прохода по текущей директории для поиска файлов с таким именем
                foreach(QFileInfo files, window.GetQDir().entryInfoList(QDir::Files|QDir::NoDotAndDotDot, QDir::Name))
                {
                    if(files.fileName()==fileName)  //если файл с таким именем есть
                    {
                        fileExists=true;    //установка флаг на истинное значение
                        break;
                    }
                }
                if(!fileExists) //если файлов с таким именем нет
                {
                        //установка пути файла
                        if(!file->Copy(filePath, copyPath))   //если копирование не произошло
                            throw PerformationException( "Copy File", "The operation <<Copy>> was not perfomed!");
                        //очистка пути файла
                        filePath=fileName="";

                }
                else throw PerformationException( "Copy File","A file with this name exists!");
            }
        }
        else if(fileName=="" && dirName!="" && linkName=="")    //если выбрана директория
        {
            QFileInfoList copyList=QFileInfoList(); //создание контейнера для хранения внутренних файлов выбранной директории
            RecursiveCopyList(qDir, copyList);   //рекурсивное наполнение контейнера внутренними файлами директории
            AdditionalWindow window;
            window.exec();  //метод выполняет появление дополнительного окна для копирования
            if(window.Cancel()) //если операция отменена
                throw PerformationException( "Copy Directory", "The operation was canceled!");
            else
            {
                QDir qNewDir=window.GetQDir();
                bool dirExists=false;    //флаг существования директорий с таким именем
                //проход по выбранной директории для поиска директории с именем копируемой
                foreach(QFileInfo dirs, qNewDir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot, QDir::Name))
                {
                    if(dirs.fileName()==dirName)        //если диреткория существует
                    {
                        dirExists=true; //установка флага на истинное значение
                        break;
                    }
                }
                if(!dirExists)  //если директорий с таким именем нет
                {
                    qNewDir.mkdir(dirName);    //создание копии директории по выбранному пути
                    qNewDir.cd(dirName);
                    //цикл копирования элементов контейнера в созданную директорию
                    foreach(QFileInfo info, copyList)
                    {
                        QString copyPath = info.filePath().replace(qDir.absolutePath(), qNewDir.absolutePath());    //создание пути для копирования
                        //если файл - копируем в файл
                        if(info.isFile()) //если текущий элемент контейнера - файл
                        {
                            if(!file->Copy(info.absoluteFilePath(), copyPath))       //если копирование не выполнено
                                throw PerformationException( "Copy File", "The operation <<Copy>> was not perfomed!");
                        }
                        if(info.isDir())    //если текущий элемент - директория
                        {
                            if(!dir->Copy(info.dir().dirName(), copyPath)) //если копирование не выполнено
                                throw PerformationException( "Copy Directory", "The operation <<Copy>> was not perfomed!");
                        }
                        if(info.isSymLink())
                        {
                            if(!link->Copy(info.absoluteFilePath(), copyPath))       //если копирование не выполнено
                                throw PerformationException( "Copy Symbol Link", "The operation <<Copy>> was not perfomed!");
                        }
                    }
                    filePath=fileName="";

                        //очистка пути директории
                    dirPath=dirName="";


                    linkPath=linkName="";

                }
                else throw PerformationException( "Copy Directory", "A directory with this name exists!");
                //очистка пути файла
            }
        }
        else if(fileName=="" && dirName=="" && linkName!="")    //если выбран файл
        {
            AdditionalWindow window;
            window.exec();  //метод выполняет появление дополнительного окна для копирования
            if(window.Cancel()) //если операция отменена
                throw PerformationException( "Copy Symbol Link", "The operation was canceled!");
            else
            {
                QString copyPath = window.GetQDir().absolutePath().append("/").append(linkName);    //создание пути для копирования
                bool linkExists=false;  //флаг существования файла с таким именем
                //цикл прохода по текущей директории для поиска файлов с таким именем
                foreach(QFileInfo links, window.GetQDir().entryInfoList(QDir::Files|QDir::NoDotAndDotDot, QDir::Name))
                {
                    if(links.fileName()==linkName)  //если файл с таким именем есть
                    {
                        linkExists=true;    //установка флаг на истинное значение
                        break;
                    }
                }
                if(!linkExists) //если файлов с таким именем нет
                {
                            //установка пути файла
                        if(!link->Copy(linkPath, copyPath))   //если копирование не произошло
                            throw PerformationException( "Copy Symbol Link", "The operation <<Copy>> was not perfomed!");
                        //очистка пути файла
                        linkPath=linkName="";

                }
                else throw PerformationException( "Copy Symbol Link","A symbol link with this name exists!");
            }
        }
    }
    catch(ChoiseException error)
    {
        error.GetException(this);
    }
    catch(PerformationException error)
    {
        error.GetException(this);
    }
    catch (...)
    {
        QMessageBox::warning(this, "Copy", "Unknown error! Please try again!");
    }
}

void MainWindow::on_btnReplace_clicked()    //слот нажатия на кнопку "Replace"
{
    QDir qDir=QDir(dirPath);
    try {
        if(fileName=="" && dirName=="" && linkName=="") //если не выбран ни один объект
            throw ChoiseException( "Replace", "You was not choose a file or a directory! Please try again");
        else if(fileName!="" && dirName=="" && linkName=="")    //если выбран файл
        {
            AdditionalWindow window;
            window.exec();  //метод выполняет появление дополнительного окна для перемещения
            if(window.Cancel()) //если операция отменена
                throw PerformationException( "Replace File", "The operation was canceled!");
            else
            {
                QString newPath = window.GetQDir().absolutePath().append("/").append(fileName);  //создание пути для перемещения
                bool fileExists=false;  //флаг существования файла с таким именем
                //цикл прохода по текущей директории для поиска файлов с таким именем
                foreach(QFileInfo files, window.GetQDir().entryInfoList(QDir::Files|QDir::NoDotAndDotDot, QDir::Name))
                {
                    if(files.fileName()==fileName)  //если файл с таким именем есть
                    {
                        fileExists=true;     //установка флаг на истинное значение
                        break;
                    }
                }
                if(!fileExists) //если файлов с таким именем нет
                {
                             //установка пути файла
                        if(!file->Copy(filePath, newPath))    //если копирование не произошло
                            throw PerformationException( "Replace File", "The operation <<Copy>> was not perfomed!");
                        if(!file->Delete(filePath)) //если удаление не произошло
                            throw PerformationException( "Replace File", "The operation <<Delete>> was not perfomed!");
                        //очистка пути файла
                        filePath=fileName="";

                }
                else throw PerformationException( "Replace File", "A file with this name exists!");
            }
        }
        else if(fileName=="" && dirName!="" && linkName=="")     //если выбрана директория
        {
            QFileInfoList replaceList=QFileInfoList(); //создание контейнера для хранения внутренних файлов выбранной директории
            RecursiveCopyList(qDir, replaceList);   //рекурсивное наполнение контейнера внутренними файлами директории
            AdditionalWindow window;
            window.exec();  //метод выполняет появление дополнительного окна для копирования
            if(window.Cancel()) //если операция отменена
                throw PerformationException( "Replace Directory", "The operation was canceled!");
            else
            {
                QDir qNewDir=window.GetQDir();
                bool dirExists=false;    //флаг существования директорий с таким именем
                //проход по выбранной директории для поиска директории с именем копируемой
                foreach(QFileInfo dirs, qNewDir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot, QDir::Name))
                {
                    if(dirs.fileName()==dirName)        //если диреткория существует
                    {
                        dirExists=true; //установка флага на истинное значение
                        break;
                    }
                }
                if(!dirExists)  //если директорий с таким именем нет
                {
                    qNewDir.mkdir(dirName);    //создание копии директории по выбранному пути
                    qNewDir.cd(dirName);
                    //цикл копирования элементов контейнера в созданную директорию
                    foreach(QFileInfo info, replaceList)
                    {
                        QString replacePath = info.filePath().replace(qDir.absolutePath(), qNewDir.absolutePath());    //создание пути для копирования
                        //если файл - копируем в файл
                        if(info.isFile()) //если текущий элемент контейнера - файл
                        {
                                if(!file->Copy(info.absoluteFilePath(), replacePath))       //если копирование не выполнено
                                    throw PerformationException( "Replace File", "The operation <<Copy>> was not perfomed!");
                        }
                        if(info.isDir())    //если текущий элемент - директория
                        {
                                if(!dir->Copy(info.dir().dirName(), replacePath)) //если копирование не выполнено
                                    throw PerformationException( "Replace Directory", "The operation <<Copy>> was not perfomed!");
                        }
                        if(info.isSymLink())
                        {
                            if(!link->Copy(info.absoluteFilePath(), replacePath))       //если копирование не выполнено
                                throw PerformationException( "Replace Symbol Link", "The operation <<Copy>> was not perfomed!");
                        }
                    }

                    if(!qDir.isEmpty())  //если директория не пуста
                    {
                        if(!RecursiveDelete(qDir, file, dir, link)) //если внутренние файлы не удалены
                            throw PerformationException( "Replace Directory", "The operation <<Delete>> was not perfomed!");
                    }
                    if(qDir.isEmpty())   //если директория пуста
                    {
                        //установка пути директории
                        if(!dir->Delete(dirPath))   //если удаление не выпонено
                            throw PerformationException( "Replace Directory", "The operation <<Delete>> was not perfomed!");
                        //очистка пути директории
                        dirPath=dirName="";
                    }
                    filePath=fileName="";
                    dirPath=dirName="";
                    linkPath=linkName="";
                }
                else throw PerformationException( "Replace Directory", "A directory with this name exists!");
            }
        }
        else if(fileName=="" && dirName=="" && linkName!="")    //если выбран файл
        {
            AdditionalWindow window;
            window.exec();  //метод выполняет появление дополнительного окна для перемещения
            if(window.Cancel()) //если операция отменена
                throw PerformationException( "Replace Symbol Link", "The operation was canceled!");
            else
            {
                QString newPath = window.GetQDir().absolutePath().append("/").append(linkName);  //создание пути для перемещения
                bool linkExists=false;  //флаг существования файла с таким именем
                //цикл прохода по текущей директории для поиска файлов с таким именем
                foreach(QFileInfo links, window.GetQDir().entryInfoList(QDir::Files|QDir::NoDotAndDotDot, QDir::Name))
                {
                    if(links.fileName()==linkName)  //если файл с таким именем есть
                    {
                        linkExists=true;     //установка флаг на истинное значение
                        break;
                    }
                }
                if(!linkExists) //если файлов с таким именем нет
                {
                             //установка пути файла
                        if(!link->Copy(linkPath, newPath))    //если копирование не произошло
                            throw PerformationException( "Replace Symbol Link", "The operation <<Copy>> was not perfomed!");
                        if(!link->Delete(linkPath)) //если удаление не произошло
                            throw PerformationException( "Replace Symbol Link", "The operation <<Delete>> was not perfomed!");
                        //очистка пути файла
                        linkPath=linkName="";

                }
                else throw PerformationException( "Replace Symbol Link", "A symbol link with this name exists!");
            }
        }
    }
    catch(ChoiseException error)
    {
        error.GetException(this);
    }
    catch(PerformationException error)
    {
        error.GetException(this);
    }
    catch (...)
    {
        QMessageBox::warning(this, "Replace", "Unknown error! Please try again!");
    }
}

void MainWindow::on_btnRename_clicked()  //слот нажатия на кнопку "Rename"
{
    QDir qDir=QDir(model->filePath(ui->lvSource->rootIndex()));  //если не выбран ни один объект
    try {
        if(qDir.absolutePath()=="/home/h4thqewjtch/qt/build-File-Desktop-Debug")   //если это корневая директория
            QMessageBox::warning(this,"Rename", "You are in a root directory! Please choose an another directory");
        else
        {
            if(filePath=="" && dirPath=="" && linkPath=="") //если не выбран ни один объект
            {
                throw ChoiseException( "Rename", "You was not choose a file or a directory! Please try again");
            }
            else if(filePath!="" && dirPath=="" && linkPath=="")    //если выбран файл
            {
                RenameWindow name;
                name.exec();    //метод выполняет появление окна для переименования файла
                if(name.GetName()=="")  //если имя не введено
                    throw ExceptionEmpty( "Rename File", "A new file name is empty! Please try again");
                else
                {
                    QString newPath=qDir.absolutePath().append("/").append(name.GetName());  //создание нового пути с учетом переименования
                    bool fileExists=false;  //флаг существования файла с таким именем
                    //цикл прохода по текущей директории для поиска файлов с таким именем
                    foreach(QFileInfo files, qDir.entryInfoList(QDir::Files|QDir::NoDotAndDotDot, QDir::Name))
                    {
                        if(files.fileName()==name.GetName())     //если файл с таким именем есть
                        {
                            fileExists=true;    //установка флаг на истинное значение
                            break;
                        }
                    }
                    if(!fileExists)//если файлов с таким именем нет
                    {
                                 //установка пути файла
                            if(!file->Rename(filePath, newPath))  //если переименование не произошло
                                throw PerformationException( "Rename File", "The operation <<Rename>> was not perfomed!");
                            //очистка пути файла
                            filePath=fileName="";

                    }
                    else throw PerformationException( "Rename File", "A file with this name exists!");
                }
            }
            else if(filePath=="" && dirPath!="" && linkPath=="")    //если выбрана директория
            {
                RenameWindow name;
                name.exec();     //метод выполняет появление окна для переименования директории
                if(name.GetName()=="")  //если имя не введено
                    throw  ExceptionEmpty("Rename Directory", "A new directory name is empty! Please try again");
                else
                {
                    QString newPath=qDir.absolutePath().append("/").append(name.GetName());  //создание нового пути с учетом переименования
                    bool dirExists=false;//флаг существования директории с таким именем
                    //цикл прохода по выбранной директории для поиска директории с таким именем
                    foreach(QFileInfo dirs, qDir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot, QDir::Name))
                    {
                        if(dirs.fileName()==name.GetName()) //если директория с таким именем есть
                        {
                            dirExists=true; //установка флаг на истинное значение
                            break;
                        }
                    }
                    if(!dirExists)  //если директорий с таким именем нет
                    {
                           //установка пути директории
                        if(!dir->Rename(dirPath, newPath))    //если переименование не произошло
                            throw PerformationException("Rename Directory", "The operation <<Rename>> was not perfomed!");
                        //очистка пути директории
                        dirPath=dirName="";

                    }
                    else throw PerformationException( "Rename Directory", "A directory with this name exists!");
                }
            }
            else if(filePath=="" && dirPath=="" && linkPath!="")    //если выбран файл
            {
                RenameWindow name;
                name.exec();    //метод выполняет появление окна для переименования файла
                if(name.GetName()=="")  //если имя не введено
                    throw ExceptionEmpty( "Rename Symbol Link", "A new symbol link name is empty! Please try again");
                else
                {
                    QString newPath=qDir.absolutePath().append("/").append(name.GetName());  //создание нового пути с учетом переименования
                    bool linkExists=false;  //флаг существования файла с таким именем
                    //цикл прохода по текущей директории для поиска файлов с таким именем
                    foreach(QFileInfo links, qDir.entryInfoList(QDir::Files|QDir::NoDotAndDotDot, QDir::Name))
                    {
                        if(links.fileName()==name.GetName())     //если файл с таким именем есть
                        {
                            linkExists=true;    //установка флаг на истинное значение
                            break;
                        }
                    }
                    if(!linkExists)//если файлов с таким именем нет
                    {
                                 //установка пути файла
                            if(!link->Rename(linkPath, newPath))  //если переименование не произошло
                                throw PerformationException( "Rename Symbol Link", "The operation <<Rename>> was not perfomed!");
                            //очистка пути файла
                            linkPath=linkName="";

                    }
                    else throw PerformationException( "Rename Symbol Link", "A symbol link with this name exists!");
                }
            }
        }
    }
    catch(ChoiseException error)
    {
        error.GetException(this);
    }
    catch(PerformationException error)
    {
        error.GetException(this);
    }
    catch(ExceptionEmpty error)
    {
        error.GetException(this);
    }
    catch (...)
    {
        QMessageBox::warning(this, "Rename", "Unknown error! Please try again!");
    }

}

void MainWindow::on_lineSearch_textEdited(const QString &arg1)  //слот ввода имени для поиска
{
    window.SetName(arg1);
}

void MainWindow::on_btnSearch_clicked() //слот нажатия на кнопку "Search"
{
    QDir dir=QDir(model->filePath(ui->lvSource->rootIndex()));  //получение текущей директории
    window.Search(dir); //поиск по имени
    window.SetUi(); //передача результатов в окно отображения
    window.exec();  //выполнение появления онка с резуьтатами поиска
    window.ResetUi();   //очистка контейнера с результатами и окна отображения результатов
}

void MainWindow::on_listWidget_currentTextChanged(const QString &currentText)   //слот копирования текущего местоположения в буфер обмена
{
    QClipboard* pcb = QApplication::clipboard();    //создание объекта для взаимодействия с буфером обмена
    pcb->setText(currentText, QClipboard::Clipboard);   //копирование выбранного текста в буфер обмена
}
