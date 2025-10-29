// WARNING: Set the build definition correctly before running
#define TEST_BUILD
#define DEBUG false

#ifdef GAS_BUILD
    #include "nodes/node_gas.h"
#endif
#ifdef PIR_BUILD
    #include "nodes/node_pir.h"
#endif
#ifdef PLANT_BUILD
    #include "nodes/node_plant.h"
#endif
#ifdef LIVROOM_BUILD
    #include "nodes/node_livroom.h"
#endif

#ifdef TEST_BUILD
    #include "nodes/node_test.h"
#endif