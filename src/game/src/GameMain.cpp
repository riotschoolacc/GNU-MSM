// GameMain.cpp
#include "../../sys/src/Singleton.h"
#include "../../sys/src/HTTPRequest.h"
#include <string>

string BBB_AUTH2_SERVER = "https://auth.bbbgame.net/auth/api/";

std::string BBB_AUTH_SERVER = "https://msmpc.bbbgame.net/";
std::string BBB_AUTH_MOBILE_SERVER = "https://msm-auth.bbbgame.net/";

bool MOBILE = false;

std::string PREGAME_SETUP = BBB_AUTH_SERVER + "pregame_setup.php";

void BBBAuth2() {
  // TODO 
  
  try
  {
      http::Request request{BBB_AUTH2_SERVER + "anon_account/"};
      const string body = "g=27";
      const auto response = request.send("POST", body, {
          {"Content-Type", "application/x-www-form-urlencoded"}
      });
      std::cout << std::string{response.body.begin(), response.body.end()} << '\n';
  }
  catch (const std::exception& e)
  {
      std::cerr << "Request failed, error: " << e.what() << '\n';
  }  
}

int main()
{
    if (MOBILE == true) {
        PREGAME_SETUP = BBB_AUTH_MOBILE_SERVER + "pregame_setup.php";
    }
    
    sys::Engine engine(1280, 760);
    return 0;
}
