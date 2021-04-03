#include <iostream>
#include <string>
#include <windows.h>
#include <winuser.h>
#include <vector>


#define IMAGE 2

#if IMAGE == 1
/*std::string image = "                    "
                    "  ###          ###  "
                    " ##### ###### ##### "
                    " ######      ###### "
                    " ####          #### "
                    "  ##            ##  "
                    "   #  ###  ###  #   "
                    "  #  ####  ####  #  "
                    "  #  # ##  ## #  #  "
                    "  #  ####  ####  #  "
                    "  #  ###    ###  #  "
                    "  #              #  "
                    "  #     ####     #  "
                    "   #     ##     #   "
                    "   #   #    #   #   "
                    "    #   ####   #    "
                    "     ##      ##     "
                    "       ######       "
                    "                    ";
int imn = 19;
int imm = 20;
                    */
#endif

#if IMAGE == 2


std::string image = "                                        "
                    "     #####                              "
                    "        ##                              "
                    "        ##   ######  ##     ##  ######  "
                    "        ##        ##  ##   ##        ## "
                    "  ##    ##   #######   ## ##    ####### "
                    "  ##    ##  ##    ##    ###    ##    ## "
                    "   ######    #######     #      ####### "
                    "                                        ";

int imn = 9;
int imm = 40;
#endif

#if IMAGE == 3


/*std::string image = "                                       "
                    "                                   ##  "
                    "                                  #  # "
                    "                                     # "
                    "                                    #  "
                    "                                   #   "
                    "  #####        ##     ##   ####   #    "
                    "  #####        ###   ###  ######  #### "
                    "  ##           #### ####  ##   #       "
                    "  ##           ## ### ##  ##           "
                    "  #####  ####  ##  #  ##  ##           "
                    "  #####  ####  ##     ##  ##           "
                    "  ##           ##     ##  ##           "
                    "  ##     ####  ##     ##  ##   #       "
                    "  #####  ####  ##     ##  ######       "
                    "  #####        ##     ##   ####        "
                    "                                       ";

int imn = 16;
int imm = 39;*/
#endif





std::vector<std::vector<std::string>> imageToCommands(std::string img, int n, int m, int offsetY, int offsetX) {

    std::vector<std::vector<std::string>> result;
    std::vector<std::string> tempVector;

    if(img.length() != n*m)
        return  result;
    std::cout << "good" << std::endl;

    for (int i = 0; i < n; ++i) {

        for (int j = 0; j < m; ++j) {
            if(img[i*m+j] == '#')
                tempVector.push_back("/set " + std::to_string(i+1+offsetY)+ " " + std::to_string(j+1+offsetX));
            else
                tempVector.push_back("/clr " + std::to_string(i+1+offsetY)+ " " + std::to_string(j+1+offsetX));
        }
        result.push_back(tempVector);
        tempVector.clear();
    }
    return result;
}


void Clipboard(const char* str, int length)
{
    HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE, length * sizeof(CHAR*));
    LPVOID gl = GlobalLock(h);

    memcpy(gl, str, length * sizeof(CHAR*));

    GlobalUnlock(gl);

    OpenClipboard(NULL); // hWnd is set to NULL
    EmptyClipboard(); // Sets clipboard ownership to someone don't know
    SetClipboardData(CF_TEXT, h);  // Successfully data to clipboard!
    CloseClipboard();
}


void pasteFromClipboard() {
    INPUT inputs[2];
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_CONTROL;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 0x56;

    SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

    Sleep(5);
    inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
}

void enter(){
    INPUT inputs[1];
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.dwFlags = 0;
    inputs[0].ki.wVk = VK_RETURN;
    SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

    Sleep(5);
    inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

}

void click() {
    mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

struct BIGCOORD {
    int X;
    int Y;
};

static void show_usage()
{
    std::cerr << "Use: " << "progname.exe " << " <option(s)>" << std::endl;
    std::cout << "Options:\n"
              << "-h,--help\tShow this help message\n\n"
              << "-r NUM1 NUM2\tgo throug raws NUM1-NUM2 War: NUM1 > NUM2, NUM1,NUM2 in {1;"<< imn <<"}\"\"\n\n"
              << "-c NUM1 NUM2\tgo throug columns NUM1-NUM2 War: NUM1 > NUM2, NUM1,NUM2 in {1;" << imm << "}\"\"\n\n"
              << "\t for single raw/column NUM1 = NUM2\"\"\n\n"
              << std::endl;
}


int main(int argc, char **argv) {

    int delay = 150;

    int lowerLimit = 0;
    int upperLimit = 0;
    bool througColumns = false;
    bool througRaws = false;


    if(argc == 1) {
        througRaws = true;
        lowerLimit = 1;
        upperLimit = imn;
    }
    else if(argc == 2) {
        std::string arg = argv[1];
        if ((arg == "-h") || (arg == "--help")) {
            show_usage();
            return 0;
        }
    }else if(argc == 4) {
        std::string arg = argv[1];
         if (arg == "-c") {
             througColumns = true;
             lowerLimit = std::stoi(argv[2]);
             upperLimit= std::stoi(argv[3]);


         } else if (arg == "-r") {
             througRaws = true;
             lowerLimit = std::stoi(argv[2]);
             upperLimit= std::stoi(argv[3]);
         }
    } else {
        show_usage();
        return 1;
    }

    if(lowerLimit > upperLimit) {
        std::cout << "NUM1 must be less than NUM2";
        return 3;
    }

    if(througRaws) {
        if (lowerLimit <0 || upperLimit > imn) {
            std::cout << "Check limits";
            return 2;
        }
    }

    if(througColumns) {
        if (lowerLimit <0 || upperLimit > imm) {
            std::cout << "Check limits";
            return 3;
        }
    }


    std::vector<std::vector<std::string>> commands = imageToCommands(image, imn, imm,40,30);


    int n = commands.size();
    int m = commands[0].size();
    

    if(n != imn || m != imm) {
        std::cout << "fuck" << std::endl;
        return 4;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            std::cout << commands[i][j] << ' ';
        }

    }
    std::cout << std::endl;


    Sleep(7000);


    lowerLimit -= 1;
    upperLimit -= 1;


    for (int k = 0; k < 4; ++k) {
        if(througRaws){
            for (int i = 0; i < n; ++i) {
                if(i >= lowerLimit && i <= upperLimit) {
                    for (int j = 0; j < m; ++j) {

                        Clipboard(commands[i][j].c_str(), commands[i][j].length());

                        // SetCursorPos(1000, 925);
                        click();
                        Sleep(5);
                        click();
                        Sleep(5);
                        pasteFromClipboard();
                        Sleep(10);
                        enter();
                        Sleep(delay);
                    }
                }
            }
           // return 0;
        }


        if(througColumns) {
            for (int i = 0; i < m; ++i) {
                if(i >= lowerLimit && i <= upperLimit) {
                    for (int j = 0; j < n; ++j) {

                        Clipboard(commands[j][i].c_str(), commands[j][i].length());
                        // SetCursorPos(1000, 925);
                        click();
                        Sleep(5);
                        click();
                        Sleep(5);
                        pasteFromClipboard();
                        Sleep(10);
                        enter();
                        Sleep(delay);
                    }
                }
            }
           // return 0;
        }
    }
  /*  if(througRaws){
        for (int i = 0; i < n; ++i) {
            if(i >= lowerLimit && i <= upperLimit) {
                for (int j = 0; j < m; ++j) {

                    Clipboard(commands[i][j].c_str(), commands[i][j].length());

                    // SetCursorPos(1000, 925);
                    click();
                    Sleep(5);
                    click();
                    Sleep(5);
                    pasteFromClipboard();
                    Sleep(10);
                    enter();
                    Sleep(delay);
                }
            }
        }
        return 0;
    }


    if(througColumns) {
        for (int i = 0; i < m; ++i) {
            if(i >= lowerLimit && i <= upperLimit) {
                for (int j = 0; j < n; ++j) {

                    Clipboard(commands[j][i].c_str(), commands[j][i].length());
                    // SetCursorPos(1000, 925);
                    click();
                    Sleep(5);
                    click();
                    Sleep(5);
                    pasteFromClipboard();
                    Sleep(10);
                    enter();
                    Sleep(delay);
                }
            }
        }
        return 0;
    }
*/


    return 0;
}