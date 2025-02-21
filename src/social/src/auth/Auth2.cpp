// Auth2.cpp
#include <string>;
#include "../../sys/src/HTTPRequest.h"

string BBB_AUTH2_SERVER = "https://auth.bbbgame.net/auth/api/";

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
