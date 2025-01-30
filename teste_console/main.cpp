
#define GPP_DOUBLE_PRECISION
#include <iostream>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include<gpp_physics/math/math.h>
#include"g_start.h"
#include"gjk.h"
#include"hdm.h"

using namespace gpp;
using namespace std;

int main() {
G_STARTER gst;

CommandFactory fr;
fr.execute("createAccount");
fr.execute("login");

logger::info("Programa finalizado!");
    return 0;
}
