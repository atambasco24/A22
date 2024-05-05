GUIDE TO THE VERSIONS

V1: The first version. This integration is dependent on a circuit that uses 2 mosfets connected to the arduino. 
    This turned out to actually be really stupid because I can just run a line directly from the battery through the switch to the motors. It doesn't need to be regulated by the arduino, and also keeps down the % of time the processor is actively being used. 
    I also made the wiring diagram very poorly, including saving it as a png with transparent background so the visibility is absolute garbage. oops. 
    The wiring diagram uses the "classic" icon of MOSFETS to illustrate simplicity (I am not an electrical enginneer), but it turns out the MOSFETS I used that are included with the Arduino are... weird? Turns out they use the middle leg as the gate instead
    of how they are traditionally wired with the left leg as the gate. 
        SIMPLIFIED NOTES
        -first version created
        -its shit
        -wiring diagram created
        -also shit

V3: I skipped V2 just because I felt like it.
    This version reflects the obvious problems introduced in V1. The entire circuit was rewired to reflect the decision to integrate the motors without a MOSFET. 
      -removed revPin and motorPin   
      -
