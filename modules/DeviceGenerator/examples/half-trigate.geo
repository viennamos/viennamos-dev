# Half of a tri-gate transistor with gate length 18nm and equivalent oxide thickness 2nm.
algebraic3d
solid source1 = orthobrick (-54.0, 0, -43.2; -32.4, 21.6, 0);
solid source2 = orthobrick (-32.4, 0, -43.2;  -9.0, 10.8, 0) -maxh=5.4;
solid source  = source1 or source2;

solid channel_horiz = orthobrick (-9, 0.0,  -2.0; 9, 10.8,  0) -maxh=1;
solid channel_vert  = orthobrick (-9, 8.8, -43.2; 9, 10.8, -2) -maxh=1;
solid channel  = channel_horiz or channel_vert;

solid drain1 = orthobrick (32.4, 0, -43.2; 54.0, 21.6, 0);
solid drain2 = orthobrick ( 9.0, 0, -43.2; 32.4, 10.8, 0) -maxh=5.4;
solid drain  = drain1 or drain2;

#oxide:
solid ox_leftleft = orthobrick (-10.8, 12.8, -43.2; 10.8, 43.2, -33.2);
solid ox_left     = orthobrick (-10.8, 10.8, -43.2; 10.8, 12.8,   0.0) -maxh=1;
solid ox_top      = orthobrick (-10.8,  0.0,   0.0; 10.8, 12.8,   2.0) -maxh=1;
solid ox          = ox_leftleft or ox_left or ox_top;

#gate:
solid gate_left = orthobrick(-10.8, 12.8, -33.2; 10.8, 43.2,  2);
solid gate_top  = orthobrick(-10.8,  0.0,   2.0; 10.8, 23.6, 20);
solid gate      = gate_left or gate_top;

#body
solid channel_core = orthobrick (-9, 0,  -43.2;  9,  8.8,  -2.0);
solid body2        = orthobrick(-54, 0, -129.6; 54, 43.2, -43.2); 
solid body = body2 or channel_core;

####### Contacts

solid source_contact  = orthobrick (-54.0, 0,    0.0; -32.4, 21.6,   10.0);
solid  drain_contact  = orthobrick ( 32.4, 0,    0.0;  54.0, 21.6,   10.0);
solid   body_contact  = orthobrick (-54.0, 0, -139.6;  54.0, 43.2, -129.6); 



tlo source  -col=[1,0,0];    #1
tlo channel -col=[0.6,0,0];  #2
tlo drain   -col=[1,0,0];    #3
tlo ox      -col=[0,1,0];    #4
tlo gate    -col=[0,0,1];    #5
tlo body    -col=[1,1,0];    #6

tlo source_contact    -col=[0.5,0.5,0.5];    #7
tlo  drain_contact    -col=[0.5,0.5,0.5];    #8
tlo   body_contact    -col=[0.5,0.5,0.5];    #9

#tests:
#solid test1 = ox and gate;
#solid test2 = ox and channel;

#tlo test1;
#tlo test2;
