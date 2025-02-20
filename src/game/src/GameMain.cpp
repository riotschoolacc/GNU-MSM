// GameMain.cpp
#include "../../sys/src/Singleton.h"
#include <string>

string BBB_AUTH_SERVER = "https://msmpc.bbbgame.net/";
string BBB_AUTH_MOBILE_SERVER = "https://msm-auth.bbbgame.net/";

string BBB_AUTH2_SERVER = "https://auth.bbbgame.net/auth/api/";

bool MOBILE = false;

string PREGAME_SETUP = BBB_AUTH_SERVER + "pregame_setup.php";
int main()
{
    if (MOBILE == true) {
        PREGAME_SETUP = BBB_AUTH_MOBILE_SERVER + "pregame_setup.php";
    }
    
    sys::Engine engine(1280, 760);
    return 0;
}
