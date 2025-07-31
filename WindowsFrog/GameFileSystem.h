#pragma once
#include "Structures.h"

wstring StrReplace(wstring* str, wstring namestr) {
    return str->replace(str->find(namestr), namestr.length(), L"");
}

//новая фича создание, читалка файлов формата Svg и преобразование ее данных в игровой уровень 
//создание карт можно делать в редакторе Adobe Photoshop (*2023 версия на которой я проверял)

void LoadSVGDataMap(const wstring NameFileSVG, const RECT& rc) {
    wifstream file;

    vector <vector<wstring>> dS{
        {},
        {},
        {},
        {},
        {},
        {}
    }; // здесть хронятся основные данные имя объекта, x, y

    vector <vector<wstring>> bufferData{ {},{} }; // здесть хронятся width, height объекта

    wstring optionArr[]{ L"id=", L"x=", L"y=" ,L"width=", L"height=",L"xlink:href=" };

    file.open(NameFileSVG + L".svg");


    //запуск чтения файла
    if (file.is_open()) {
        wstring str =L"";
        while (!file.eof()) {
            file >> str;

            for (int i = 0; i < 6; i++) {
                if (!str.find(optionArr[i])) {
                    StrReplace(&str, optionArr[i]);
                    StrReplace(&str, L"\"");
                    StrReplace(&str, L"\"");

                    //исключения
                    if (!str.find(L"walls_f") || !str.find(L"background") || !str.find(L"image")) {
                        if (!str.find(L"walls_f")) {
                            dS[i].push_back(L"empty");
                            dS[5].push_back(L"error image");
                        }

                        dS[1].push_back(L"0");
                        dS[2].push_back(L"0");
                    }

                    //сортировка данных
                    if (str.find(L"walls_f")) {
                        if (str.find(L"data:")) {
                            dS[i].push_back(str);
                        }
                        else {
                            dS[5].push_back(L"error image");
                        }
                    }
                }
            }
        }

        //присвоение и пересборка массива новыми данными ширины и высоты объектов для тех объектов где эти данные записаны в шаблон #image
        for (int i = 0; i < dS[0].size(); i++) {
            if (dS[5][i] != L"error image") {
                for (int j = 0; j < dS[0].size(); j++) {
                    if (dS[5][i] == L"#" + dS[0][j] + L"/>") {
                        bufferData[0].push_back(dS[3][j]);
                        bufferData[1].push_back(dS[4][j]);
                    }
                }
            }
            else {
                bufferData[0].push_back(L"standardData");
                bufferData[1].push_back(L"standardData");
            }
        }
        int IDWidth = 0;
        int IDHeight = 0;
        for (int i = 0; i < bufferData[0].size(); i++) {
            if (bufferData[0][i] == L"standardData") {
                bufferData[0][i] = dS[3][IDWidth++];
                bufferData[1][i] = dS[4][IDHeight++];
            }
        }
    }


    //загрузка уровня на основе собранных данных
    for (int i = 0; i < dS[0].size(); i++) {
        //интрепритация стоковых данных в числовые значения типа 0.1 от разрешения экрана пользователя
        float x = stof(dS[1][i]) / rc.right;
        float y = stof(dS[2][i]) / rc.bottom;
        float width = stof(bufferData[0][i]) / rc.right;
        float height = stof(bufferData[1][i]) / rc.bottom;
        wstring nameObject = dS[0][i];


        //создание объектов пока что только для уровня 0
        if (!nameObject.find(L"walls")) {
            location[0].walls.emplace_back(x, y, width, height, rc, L"walls");
        }
        else if (!nameObject.find(L"enemy")) {
            wolf = new Wolf(x, y, width, height, rc, L"enemy1", 40, 5, 3, 0);
        }
        else if (!nameObject.find(L"racket")) {
            player = new Hero(x, y, width, height, rc, L"racket", 40, 5, 3, 0);
        }
        else if (!nameObject.find(L"background")) {
            location[0].hBack.loadBitmapWithNativeSize(nameObject);
        }
        else if (!nameObject.find(L"portal")) {
            location[0].portal.emplace_back(x, y, width, height, rc, L"racket", 1); // пока реализациия работает на переход на 1 уровень
        }
        else if (!nameObject.find(L"heal")) {
            location[0].healingFlask.emplace_back(x, y, width, height, rc, L"ball");
        }
        else if (!nameObject.find(L"spike")) {
            location[0].spike.emplace_back(x, y, width, height, rc, L"spike");
        }

    }
    file.close();
}

//class Serialization {
//public:
//    Serialization(Location_ container) { this->container = container; }
//
//    /// добавить функцию создания сохранения с нуля и загрузка мира с нулевого сохранения
//    /// добавить функцию проверки на наличие изменения данных в файле чтобы создать мир с нуля и перезаписать данные на новые
//
//    void Ser() {
//        ofile.open("SaveData.txt", ios::trunc);
//        ofile.close();
//        ofile.open("SaveData.txt", ofstream::app);
//        if (ofile.is_open())
//        {
//            ///hardcode?!?!?!?!?!?
//            for (int i = 0; i < container.healingFlask.size(); i++) {
//                ofile << "<healingFlask>" << "index=" << i << " ";
//                SortSprite(container.healingFlask[i].Sprite);
//            }
//            for (int i = 0; i < container.spike.size(); i++) {
//                ofile << "<spike>" << "index=" << i << " ";
//                SortSprite(container.spike[i].Sprite);
//            }
//            for (int i = 0; i < container.Persona.size(); i++) {
//                ofile << "<Persona>" << "index=" << i << " ";
//                SortSprite(container.Persona[i]->Sprite);
//            }
//            for (int i = 0; i < container.portal.size(); i++) {
//                ofile << "<portal>" << "index=" << i << " ";
//                SortSprite(container.portal[i].Sprite);
//            }
//            for (int i = 0; i < container.walls.size(); i++) {
//                ofile << "<walls>" << "index=" << i << " ";
//                SortSprite(container.walls[i].Sprite);
//
//            }
//        }
//        ofile.close();
//    }
//    void Deser() {
//        ifile.open("SaveData.txt");
//        if (ifile.is_open())
//        {
//            string StructureArr[]{ "<healingFlask>", "<spike>", "<Persona>", "<portal>", "<walls>", "height", "width", "xpos", "ypos" };
//            string str;
//            while (!ifile.eof()) {
//                ifile >> str;
//                for (int i = 0; i < 9; i++) {
//                    ///определение объекта по его индексу и по экземляру стуктуры
//                    if (!str.find(StructureArr[i]) &&
//                        !str.find(StructureArr[i] + "index=")) {
//                        ReplaceStructure(str, StructureArr[i]);
//                        break;
//                    }
//                    else if (!str.find(StructureArr[i]) &&
//                        !str.find(StructureArr[i] + "==")) {
//                        ReplaceProperty(str);
//                        break;
//                    }
//                }
//            }
//        }
//        ifile.close();
//    }
//private:
//    string MarkerStructure;
//    int IndexStructure;
//    Location_ container;
//    ifstream ifile;
//    ofstream ofile;
//
//    void SortSprite(sprite SprData) {
//        ofile << "height==" << SprData.height << " "
//            << "width==" << SprData.width << " "
//            << "xpos==" << SprData.x << " "
//            << "ypos==" << SprData.y << "\n";
//    }
//    void ReplaceStructure(string str, string StructureArr) {
//        StrReplace(&str, StructureArr + "index=");
//        MarkerStructure = StructureArr;
//        IndexStructure = stoi(str);
//
//    };
//    void ReplaceProperty(string StructureArr) {
//        float height = NULL;
//        float width = NULL;
//        float xpos = NULL;
//        float ypos = NULL;
//
//        if (!StructureArr.find("height")) {
//            StrReplace(&StructureArr, "height==");
//            height = stof(StructureArr);
//        }
//        else if (!StructureArr.find("width")) {
//            StrReplace(&StructureArr, "width==");
//            width = stof(StructureArr);
//        }
//        else if (!StructureArr.find("xpos")) {
//            StrReplace(&StructureArr, "xpos==");
//            xpos = stof(StructureArr);
//        }
//        else if (!StructureArr.find("ypos")) {
//            StrReplace(&StructureArr, "ypos==");
//            ypos = stof(StructureArr);
//        }
//
//        if (MarkerStructure == "<healingFlask>") {
//            if (container.healingFlask.size() != 0)
//            {
//                SwapProperty(&container.healingFlask[IndexStructure].Sprite, height, width, xpos, ypos);
//            }
//        }
//        else if (MarkerStructure == "<spike>") {
//            if (container.spike.size() != 0)
//            {
//                SwapProperty(&container.spike[IndexStructure].Sprite, height, width, xpos, ypos);
//            }
//        }
//        else if (MarkerStructure == "<Persona>") {
//            if (container.Persona.size() != 0)
//            {
//                SwapProperty(&container.Persona[IndexStructure]->Sprite, height, width, xpos, ypos);
//            }
//        }
//        else if (MarkerStructure == "<portal>") {
//            if (container.portal.size() != 0)
//            {
//                SwapProperty(&container.portal[IndexStructure].Sprite, height, width, xpos, ypos);
//            }
//        }
//        else if (MarkerStructure == "<walls>") {
//            if (container.walls.size() != 0)
//            {
//                SwapProperty(&location[0].walls[IndexStructure].Sprite, height, width, xpos, ypos);
//            }
//        }
//
//        height = NULL;
//        width = NULL;
//        xpos = NULL;
//        ypos = NULL;
//    };
//    void SwapProperty(sprite* Struct, float h, float w, float x, float y) {
//        if (h != NULL) { Struct->height = h; }
//        if (w != NULL) { Struct->width = w; }
//        if (x != NULL) { Struct->x = x; }
//        if (y != NULL) { Struct->y = y; }
//    };
//
//};
