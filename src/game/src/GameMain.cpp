// GameMain.cpp
#include "../../sys/src/Singleton.h"
#include <string>

std::string BBB_AUTH_SERVER = "https://msmpc.bbbgame.net/";
std::string BBB_AUTH_MOBILE_SERVER = "https://msm-auth.bbbgame.net/";

bool MOBILE = false;

std::string PREGAME_SETUP = BBB_AUTH_SERVER + "pregame_setup.php";
int main()
{
    if (MOBILE == true) {
        PREGAME_SETUP = BBB_AUTH_MOBILE_SERVER + "pregame_setup.php";
    }
    
    sys::Engine engine(1280, 760);
    return 0;
}
