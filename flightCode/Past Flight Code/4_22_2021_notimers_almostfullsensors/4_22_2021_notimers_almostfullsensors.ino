//GND TELEMTRY FORMAT
//teamId,missionTime,packetCount,packetType,mode,sp1Released,sp2Released,altitude,temp,voltage,gpsTime,gpsLat,gpsLong,gpsAlt,gpsSats,flightStage,sp1PacketCount,sp2PacketCount,lastCommand
//GND COMMAND FORMAT
//CMD,2617,CX,PING

/*float fakeData[1325] = {100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100833,
100601.9,
100370.8,
100139.7,
99908.6,
99677.5,
99446.4,
99215.3,
98984.2,
98753.1,
98522,
98405.1,
98288.2,
98171.3,
98054.4,
97937.5,
97820.6,
97703.7,
97586.8,
97469.9,
97353,
97225.8,
97098.6,
96971.4,
96844.2,
96717,
96589.8,
96462.6,
96335.4,
96208.2,
96081,
95977.8,
95874.6,
95771.4,
95668.2,
95565,
95461.8,
95358.6,
95255.4,
95152.2,
95049,
94967.2,
94885.4,
94803.6,
94721.8,
94640,
94558.2,
94476.4,
94394.6,
94312.8,
94231,
94170,
94109,
94048,
93987,
93926,
93865,
93804,
93743,
93682,
93621,
93572.6,
93524.2,
93475.8,
93427.4,
93379,
93330.6,
93282.2,
93233.8,
93185.4,
93137,
93104.5,
93072,
93039.5,
93007,
92974.5,
92942,
92909.5,
92877,
92844.5,
92812,
92788.6,
92765.2,
92741.8,
92718.4,
92695,
92671.6,
92648.2,
92624.8,
92601.4,
92578,
92584.7,
92591.4,
92598.1,
92604.8,
92611.5,
92618.2,
92624.9,
92631.6,
92638.3,
92645,
92667.3,
92689.6,
92711.9,
92734.2,
92756.5,
92778.8,
92801.1,
92823.4,
92845.7,
92868,
92856.8,
92845.6,
92834.4,
92823.2,
92812,
92800.8,
92789.6,
92778.4,
92767.2,
92756,
92772.8,
92789.6,
92806.4,
92823.2,
92840,
92856.8,
92873.6,
92890.4,
92907.2,
92924,
92922.9,
92921.8,
92920.7,
92919.6,
92918.5,
92917.4,
92916.3,
92915.2,
92914.1,
92913,
92927.6,
92942.2,
92956.8,
92971.4,
92986,
93000.6,
93015.2,
93029.8,
93044.4,
93059,
93064.6,
93070.2,
93075.8,
93081.4,
93087,
93092.6,
93098.2,
93103.8,
93109.4,
93115,
93125.1,
93135.2,
93145.3,
93155.4,
93165.5,
93175.6,
93185.7,
93195.8,
93205.9,
93216,
93219.4,
93222.8,
93226.2,
93229.6,
93233,
93236.4,
93239.8,
93243.2,
93246.6,
93250,
93263.4,
93276.8,
93290.2,
93303.6,
93317,
93330.4,
93343.8,
93357.2,
93370.6,
93384,
93389.7,
93395.4,
93401.1,
93406.8,
93412.5,
93418.2,
93423.9,
93429.6,
93435.3,
93441,
93850.8,
97539,
94820.2,
94431.8,
94043.4,
93655,
93659.5,
93664,
93668.5,
93673,
93677.5,
93682,
93686.5,
93691,
93695.5,
93700,
93709,
93718,
93727,
93736,
93745,
93754,
93763,
93772,
93781,
93790,
93799,
93808,
93817,
93826,
93835,
93844,
93853,
93862,
93871,
93880,
93886.8,
93893.6,
93900.4,
93907.2,
93914,
93920.8,
93927.6,
93934.4,
93941.2,
93948,
93960.4,
93972.8,
93985.2,
93997.6,
94010,
94022.4,
94034.8,
94047.2,
94059.6,
94072,
94102.6,
94133.2,
94163.8,
94194.4,
94225,
94255.6,
94286.2,
94316.8,
94347.4,
94378,
94363.3,
94348.6,
94333.9,
94319.2,
94304.5,
94289.8,
94275.1,
94260.4,
94245.7,
94231,
94237.8,
94244.6,
94251.4,
94258.2,
94265,
94271.8,
94278.6,
94285.4,
94292.2,
94299,
94306.9,
94314.8,
94322.7,
94330.6,
94338.5,
94346.4,
94354.3,
94362.2,
94370.1,
94378,
94390.5,
94403,
94415.5,
94428,
94440.5,
94453,
94465.5,
94478,
94490.5,
94503,
94514.4,
94525.8,
94537.2,
94548.6,
94560,
94571.4,
94582.8,
94594.2,
94605.6,
94617,
94620.4,
94623.8,
94627.2,
94630.6,
94634,
94637.4,
94640.8,
94644.2,
94647.6,
94651,
94666.9,
94682.8,
94698.7,
94714.6,
94730.5,
94746.4,
94762.3,
94778.2,
94794.1,
94810,
94962.4,
95114.8,
95267.2,
96181.6,
96334,
96195.3,
96056.6,
95085.7,
94947,
94953.8,
94960.6,
94967.4,
94974.2,
94981,
94987.8,
94994.6,
95001.4,
95008.2,
95015,
95025.3,
95035.6,
95045.9,
95056.2,
95066.5,
95076.8,
95087.1,
95097.4,
95107.7,
95118,
95134,
95150,
95166,
95182,
95198,
95214,
95230,
95246,
95262,
95278,
95282.5,
95287,
95291.5,
95296,
95300.5,
95305,
95309.5,
95314,
95318.5,
95323,
96602.6,
96325.7,
96048.8,
95771.9,
95495,
95504.2,
95513.4,
95522.6,
95531.8,
95541,
95550.2,
95559.4,
95568.6,
95577.8,
95587,
95596.1,
95605.2,
95614.3,
95623.4,
95632.5,
95641.6,
95650.7,
95659.8,
95668.9,
95678,
95688.4,
95698.8,
95709.2,
95719.6,
95730,
95740.4,
95750.8,
95761.2,
95771.6,
95782,
95790,
95798,
95806,
95814,
95822,
95830,
95838,
95846,
95854,
95862,
95888.5,
95915,
95941.5,
95968,
95994.5,
96021,
96047.5,
96074,
96100.5,
96127,
96116.6,
96106.2,
96095.8,
96085.4,
96075,
96064.6,
96054.2,
96043.8,
96033.4,
96023,
96027.6,
96032.2,
96036.8,
96041.4,
96046,
96050.6,
96055.2,
96059.8,
96064.4,
96069,
96077.1,
96085.2,
96093.3,
96101.4,
96109.5,
96117.6,
96125.7,
96133.8,
96141.9,
96150,
96155.7,
96161.4,
96167.1,
96172.8,
96178.5,
96184.2,
96189.9,
96195.6,
96201.3,
96207,
96215.1,
96223.2,
96231.3,
96239.4,
96247.5,
96255.6,
96263.7,
96271.8,
96279.9,
96288,
96294.9,
96301.8,
96308.7,
96315.6,
96322.5,
96329.4,
96336.3,
96343.2,
96350.1,
96357,
96365.1,
96373.2,
96381.3,
96389.4,
96397.5,
96405.6,
96413.7,
96421.8,
96429.9,
96438,
96446.1,
96454.2,
96462.3,
96470.4,
96478.5,
96486.6,
96494.7,
96502.8,
96510.9,
96519,
96529.4,
96539.8,
96550.2,
96560.6,
96571,
96581.4,
96591.8,
96602.2,
96612.6,
96623,
96627.6,
96632.2,
96636.8,
96641.4,
96646,
96650.6,
96655.2,
96659.8,
96664.4,
96669,
96675.9,
96682.8,
96689.7,
96696.6,
96703.5,
96710.4,
96717.3,
96724.2,
96731.1,
96738,
96747.3,
96756.6,
96765.9,
96775.2,
96784.5,
96793.8,
96803.1,
96812.4,
96821.7,
96831,
96837.9,
96844.8,
96851.7,
96858.6,
96865.5,
96872.4,
96879.3,
96886.2,
96893.1,
96900,
96908.1,
96916.2,
96924.3,
96932.4,
96940.5,
96948.6,
96956.7,
96964.8,
96972.9,
96981,
96995,
97009,
97023,
97037,
97051,
97065,
97079,
97093,
97107,
97121,
97126.8,
97132.6,
97138.4,
97144.2,
97150,
97155.8,
97161.6,
97167.4,
97173.2,
97179,
97180.1,
97181.2,
97182.3,
97183.4,
97184.5,
97185.6,
97186.7,
97187.8,
97188.9,
97190,
97200.5,
97211,
97221.5,
97232,
97242.5,
97253,
97263.5,
97274,
97284.5,
97295,
97299.6,
97304.2,
97308.8,
97313.4,
97318,
97322.6,
97327.2,
97331.8,
97336.4,
97341,
97352.7,
97364.4,
97376.1,
97387.8,
97399.5,
97411.2,
97422.9,
97434.6,
97446.3,
97458,
97472,
97486,
97500,
97514,
97528,
97542,
97556,
97570,
97584,
97598,
97600.3,
97602.6,
97604.9,
97607.2,
97609.5,
97611.8,
97614.1,
97616.4,
97618.7,
97621,
97615.2,
97609.4,
97603.6,
97597.8,
97592,
97586.2,
97580.4,
97574.6,
97568.8,
97563,
97567.6,
97572.2,
97576.8,
97581.4,
97586,
97590.6,
97595.2,
97599.8,
97604.4,
97609,
97617.2,
97625.4,
97633.6,
97641.8,
97650,
97658.2,
97666.4,
97674.6,
97682.8,
97691,
97698,
97705,
97712,
97719,
97726,
97733,
97740,
97747,
97754,
97761,
97764.5,
97768,
97771.5,
97775,
97778.5,
97782,
97785.5,
97789,
97792.5,
97796,
97800.7,
97805.4,
97810.1,
97814.8,
97819.5,
97824.2,
97828.9,
97833.6,
97838.3,
97843,
97847.6,
97852.2,
97856.8,
97861.4,
97866,
97870.6,
97875.2,
97879.8,
97884.4,
97889,
97893.7,
97898.4,
97903.1,
97907.8,
97912.5,
97917.2,
97921.9,
97926.6,
97931.3,
97936,
97944.2,
97952.4,
97960.6,
97968.8,
97977,
97985.2,
97993.4,
98001.6,
98009.8,
98018,
98055.5,
98093,
98130.5,
98168,
98205.5,
98243,
98280.5,
98318,
98355.5,
98393,
101822.8,
101233.5,
100644.2,
100054.9,
99465.6,
98876.3,
98287,
98282.3,
98277.6,
98272.9,
98268.2,
98263.5,
98258.8,
98254.1,
98249.4,
98244.7,
98240,
98255.3,
98270.6,
98285.9,
98301.2,
98316.5,
98331.8,
98347.1,
98362.4,
98377.7,
98393,
98393,
98393,
98393,
98393,
98393,
98393,
98393,
98393,
98393,
98393,
98397.7,
98402.4,
98407.1,
98411.8,
98416.5,
98421.2,
98425.9,
98430.6,
98435.3,
98440,
98448.2,
98456.4,
98464.6,
98472.8,
98481,
98489.2,
98497.4,
98505.6,
98513.8,
98522,
98531.4,
98540.8,
98550.2,
98559.6,
98569,
98578.4,
98587.8,
98597.2,
98606.6,
98616,
98618.3,
98620.6,
98622.9,
98625.2,
98627.5,
98629.8,
98632.1,
98634.4,
98636.7,
98639,
98644.9,
98650.8,
98656.7,
98662.6,
98668.5,
98674.4,
98680.3,
98686.2,
98692.1,
98698,
98702.7,
98707.4,
98712.1,
98716.8,
98721.5,
98726.2,
98730.9,
98735.6,
98740.3,
98745,
98752.1,
98759.2,
98766.3,
98773.4,
98780.5,
98787.6,
98794.7,
98801.8,
98808.9,
98816,
98821.9,
98827.8,
98833.7,
98839.6,
98845.5,
98851.4,
98857.3,
98863.2,
98869.1,
98875,
98883.2,
98891.4,
98899.6,
98907.8,
98916,
98924.2,
98932.4,
98940.6,
98948.8,
98957,
98968.8,
98980.6,
98992.4,
99004.2,
99016,
99027.8,
99039.6,
99051.4,
99063.2,
99075,
99118.7,
99162.4,
99206.1,
99249.8,
99293.5,
99337.2,
99380.9,
99424.6,
99468.3,
99512,
99487.2,
99462.4,
99437.6,
99412.8,
99388,
99363.2,
99338.4,
99313.6,
99288.8,
99264,
99267.5,
99271,
99274.5,
99278,
99281.5,
99285,
99288.5,
99292,
99295.5,
99299,
99421.4,
99543.8,
99666.2,
99788.6,
99911,
100033.4,
100155.8,
100278.2,
100400.6,
100523,
100411.3,
100299.6,
100187.9,
100076.2,
99964.5,
99852.8,
99741.1,
99629.4,
99517.7,
99406,
99414.3,
99422.6,
99430.9,
99439.2,
99447.5,
99455.8,
99464.1,
99472.4,
99480.7,
99489,
99497.2,
99505.4,
99513.6,
99521.8,
99530,
99538.2,
99546.4,
99554.6,
99562.8,
99571,
99584.1,
99597.2,
99610.3,
99623.4,
99636.5,
99649.6,
99662.7,
99675.8,
99688.9,
99702,
99706.7,
99711.4,
99716.1,
99720.8,
99725.5,
99730.2,
99734.9,
99739.6,
99744.3,
99749,
99755,
99761,
99767,
99773,
99779,
99785,
99791,
99797,
99803,
99809,
99816.1,
99823.2,
99830.3,
99837.4,
99844.5,
99851.6,
99858.7,
99865.8,
99872.9,
99880,
99888.3,
99896.6,
99904.9,
99913.2,
99921.5,
99929.8,
99938.1,
99946.4,
99954.7,
99963,
99970.1,
99977.2,
99984.3,
99991.4,
99998.5,
100005.6,
100012.7,
100019.8,
100026.9,
100034,
100043.5,
100053,
100062.5,
100072,
100081.5,
100091,
100100.5,
100110,
100119.5,
100129,
100152.9,
100176.8,
100200.7,
100224.6,
100248.5,
100272.4,
100296.3,
100320.2,
100344.1,
100368,
100377.5,
100387,
100396.5,
100406,
100415.5,
100425,
100434.5,
100444,
100453.5,
100463,
100609.4,
100755.8,
100902.2,
101048.6,
101195,
101341.4,
101487.8,
101634.2,
101780.6,
101927,
101783,
101639,
101495,
101351,
101207,
101063,
100919,
100775,
100631,
100487,
100496.5,
100506,
100515.5,
100525,
100534.5,
100544,
100553.5,
100563,
100572.5,
100582,
100584.4,
100586.8,
100589.2,
100591.6,
100594,
100596.4,
100598.8,
100601.2,
100603.6,
100606,
100618,
100630,
100642,
100654,
100666,
100678,
100690,
100702,
100714,
100726,
100729.6,
100733.2,
100736.8,
100740.4,
100744,
100747.6,
100751.2,
100754.8,
100758.4,
100762,
100767.9,
100773.8,
100779.7,
100785.6,
100791.5,
100797.4,
100803.3,
100809.2,
100815.1,
100821,
100828.2,
100835.4,
100842.6,
100849.8,
100857,
100864.2,
100871.4,
100878.6,
100885.8,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893,
100893
};*/

//INCLUDE ALL HEADER FILES NEEDED
#include <init.h> //includes necessary libraries and initializes data vars
#include <Sensors.h> //initialize and get data from sensors
#include <SerialDefinitions.h> //initialize Serial2 and Serial3
#include <TimeFunctions.h> //function to grab time (in terms of arduino millis)
//DEFINE SENSORS
Sensors sensors(6);
TimeFunctions timeFunctions;
Servo myServo1;
Servo myServo2;

//DEFINE DELAY VARS
const int sensorDelayNum = 100;
int sensorDelayStart;
const int printDelayNum = 1000;
int printDelayStart;
const int readDelayNum = 10;
int readDelayStart;
const int altCheckDelayNum = 500;
int altCheckDelayStart;
const int gpsDelayNum = 2000;
int gpsDelayStart;

//container sensor vars, alt correction, and othe global vars are defined in init.h
//DEFINE PAYLOAD SENSOR VARS FOR RELAY

//OTHER GLOBAL VARS TO DEFINE
//int incomingByte = 0;

//DEFINE RECEIVING DATA VARS
const byte numChars = 40;
char receivedChars[numChars];
boolean newData = false;

const byte numChars2 = 50;
char receivedChars2[numChars2];
boolean newData2 = false;

float previousAlts[10];
float deltaAlt[10];
float previousAlt;
float currentAlt; 
int deltaAltSampleIndex = 0;

int sampleIndex = 0;

float bmpAltSamples[10];
float voltageSamples[10];
float rawRotRateX[10];
float rawRotRateY[10];
float rawRotRateZ[10];

const int DO_WRITE_TO_FLASH = 0;
FlashStorage(flightStageFlash, int);
FlashStorage(altCorrectionFlash, int);

int doSendData = 1;

int openLogPacketCount = 0;
float openLogAverageDeltaAlt;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  Serial1.begin(115200); //openlog
  while (!Serial1) { Serial1.println("open log aint starting"); };
  Serial1.println("begin test");
  Serial2.begin(9600); 
  while (!Serial2){ Serial1.print("xbee aint starting"); };
  Serial3.begin(9600);
  while (!Serial3){ Serial2.print("xbee2 aint starting"); };
  Serial2.println("STARTING CONTAINER SOFTWARE");
  Serial1.println("past serial begin");

  if(DO_WRITE_TO_FLASH){
    Serial1.println("ABOUT TO READ INITIAL VARS FROM FLASH");
    altCorrection = altCorrectionFlash.read();
    flightStage = flightStageFlash.read();
    Serial1.println("alt Correction");
    Serial1.println(altCorrection);
    Serial1.println("flight stage");
    Serial1.println(flightStage);
  }
  
  sensors.init();
  float dummy = sensors.getTemp();
  dummy = sensors.getPressure();
  dummy = sensors.getTemp();
  dummy = sensors.getPressure();
  
  Serial1.println("past sensor init");
  Serial1.println("Time, rotRateX, rotRateY, rotRateZ, Alt, Temp, Voltage, gpsTime, Lat, Long, gpsAlt, gpsSats, flightStage, lastCommand, altCorrection");
  
  sensorDelayStart = millis();
  printDelayStart = millis();
  readDelayStart = millis();
  altCheckDelayStart = millis();
  gpsDelayStart = millis();
  gpsDelayStart = millis();

  //delay(10000);
  //sensors.stopCamera();
  
}

//int x = 0;
int currentTs;

void loop() {
  currentTs = millis();
  
  //INTERVAL TO GET SENSOR DATA
  if((currentTs - sensorDelayStart) > sensorDelayNum){
    //ACCUIRE ALL RAW SENSOR DATA AND ADD TO ARRAYS
    tem = sensors.getTemp();
    pres = sensors.getPressure();
    //pres = fakeData[x] / 100;
    //x++;
    bmpAltSamples[sampleIndex] = 44330*(1 - pow((pres/SEALEVELPRESSURE_HPA), (1/5.255)));
    voltageSamples[sampleIndex] = sensors.getBattVoltage();
    rawRotRateX[sampleIndex] = sensors.getRotRateX();
    rawRotRateY[sampleIndex] = sensors.getRotRateY();
    rawRotRateZ[sampleIndex] = sensors.getRotRateZ();

    //PERFORM AVERAGING
    float totalAltitudes = 0;
    float totalVoltages = 0;
    float totalRotRateX = 0;
    float totalRotRateY = 0;
    float totalRotRateZ = 0;
    for(int i = 0; i < 10; i++){
      totalAltitudes += bmpAltSamples[i];
      totalVoltages += voltageSamples[i];
      totalRotRateX += rawRotRateX[i];
      totalRotRateY += rawRotRateY[i];
      totalRotRateZ += rawRotRateZ[i];
    }
    alt = totalAltitudes / 10;
    voltage = totalVoltages / 10;
    rotRate[0] = totalRotRateX / 10;
    rotRate[1] = totalRotRateY / 10;
    rotRate[2] = totalRotRateZ / 10;
    
    //CALCULATE DELTA ALT
    previousAlt = currentAlt;
    currentAlt = alt;
    if(!(currentAlt > 1000 && currentAlt < 0)){
      deltaAlt[deltaAltSampleIndex] = currentAlt - previousAlt; 
      previousAlts[deltaAltSampleIndex] = currentAlt;
    
      if(deltaAltSampleIndex == 9){
        deltaAltSampleIndex = 0;
      }else{
        deltaAltSampleIndex++;
      }
    }else{
      Serial2.println("THROWING OUT ALTITUDE: OUT OF RANGE: " + String(currentAlt));
      Serial1.println("THROWING OUT ALTITUDE: OUT OF RANGE: " + String(currentAlt));
    }

    //INCREMENT DATS SAMPLE INDEX
    if(sampleIndex == 9){
      sampleIndex = 0;
    }else{
      sampleIndex++;
    }

    Serial1.println(String(millis()) + "," + String(openLogPacketCount) + "," + String(rotRate[0]) + "," + String(rotRate[1]) + "," + String(rotRate[2]) + "," + String(alt) + "," + String(tem) +
                  "," + String(voltage) + "," + gpsTime + "," + String(gpsLat) + "," + String(gpsLong) + "," + String(gpsAlt) + "," + 
                  String(gpsSats) + "," + String(flightStage) + "," + lastCommand + "," + altCorrection + "," + String(openLogAverageDeltaAlt));
    openLogPacketCount++;

    sensorDelayStart = millis();
  }

  //INTERVAL TO GET GPS DATA
  if((currentTs - gpsDelayStart) > gpsDelayNum){
    gpsLat = sensors.getLat();
    gpsLong = sensors.getLong();
    gpsAlt = sensors.getGPSAlt();
    gpsTime = sensors.getGPSTime();
    gpsSats = sensors.getNumSats();
    gpsDelayStart = millis();

  }
  
  
  //INTERVAL TO PRINT TO SERIAL DEVICES
  if((currentTs - printDelayStart) > printDelayNum){
    printToXbee();
    printDelayStart = millis();
  }

  //INTERVAL TO READ FROM SERIAL DEVICES
  if((currentTs - readDelayStart) > readDelayNum){
     recvWithStartEndMarkers();
     showNewData();
     //delay(10);
     recvWithStartEndMarkers2();
     showNewData2();
     readDelayStart = millis();
   }

   if((currentTs - altCheckDelayStart) > altCheckDelayNum){
     altitudeCheck();
     altCheckDelayStart = millis();
   }
}

void printToXbee(){
  if(doSendData){
    missionTime = timeFunctions.getTime();
  
    Serial2.println(String(teamId) + "," + missionTime + "," + String(packetCount) + "," + packetType + "," + mode + "," + sp1Released + "," + sp2Released + "," + String(alt) + "," + String(tem) +
                  "," + String(voltage) + "," + gpsTime + "," + String(gpsLat) + "," + String(gpsLong) + "," + String(gpsAlt) + "," + String(gpsSats) + "," + String(flightStage) + "," + String(sp1PacketCount) + "," +
                  String(sp2PacketCount) + "," + lastCommand + "," + altCorrection + "," + String(openLogAverageDeltaAlt));
    packetCount += 1;
  }
}

void altitudeCheck(){ 

  //calculate the average change in altitude of the past 10 delta altitudes 
  float total;
  float averageDeltaAlt;
  for(int i = 0; i < 10; i++){
    total += deltaAlt[i];
  }
  averageDeltaAlt = total / 10;
  openLogAverageDeltaAlt = averageDeltaAlt;

  if(averageDeltaAlt < -1.0 && flightStage != 1){
    Serial2.println("transition to flight stage 1");
    Serial2.println(averageDeltaAlt);
    Serial1.println("transition to flight stage 1");
    Serial1.println(averageDeltaAlt);
    
    flightStage = 1;

    if(DO_WRITE_TO_FLASH){
      Serial2.println("WARNING: ABOUT TO WRITE TO FLASH: FLIGHT STAGE");
      flightStageFlash.write(flightStage);
    }
  }

  int shouldDeploy1 = 1;
  int shouldDeploy2 = 1;
  //if we are falling 
  if(flightStage == 1){
    
    for(int i = 0; i < 10; i++){
      if (!(previousAlts[i] <= (500 + altCorrection))){
        shouldDeploy1 = 0;
      }
      if(!(previousAlts[i] <= (400 + altCorrection))){
        shouldDeploy2 = 0;
      }
    }
    
    if(shouldDeploy1 && sp1Released == "N"){
      Serial2.println("DEPLOY PAYLOAD 1");
      Serial1.println("DEPLOY PAYLOAD 1");
      sensors.releaseServo1();
      sp1Released = "Y";
    }
    if(shouldDeploy2 && sp2Released == "N"){
      Serial2.println("DEPLOY PAYLOAD 2");
      Serial1.println("DEPLOY PAYLOAD 1");
      sensors.releaseServo2();
      sp2Released = "Y";
    }
   
  }
  /*Serial2.println("averageDeltaAlt");
  Serial2.println(averageDeltaAlt);
  Serial2.println("alt");
  Serial2.println(alt);*/
  
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (Serial2.available() > 0 && newData == false) {
        rc = Serial2.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
        String stringVersionReceivedChars;
        stringVersionReceivedChars = receivedChars;
        if(stringVersionReceivedChars == "CMD,2617,CX,PING"){
          Serial2.println("CMD_2617_CX_PING");
          lastCommand = "PING";
          
        }else if(stringVersionReceivedChars == "CMD,2617,CX,RELEASE"){
          Serial2.println("RELEASE_CMD_RECIEVED");
          lastCommand = "RELEASE";
          
          //activate servos to release
          sensors.releaseServo1();
          sensors.releaseServo2();

        }else if(stringVersionReceivedChars.substring(0, 28) == "CMD,2617,CX,SETALTCORRECTION"){
          Serial2.println("alt command recieved");
          
          String sentAltCorrect = stringVersionReceivedChars.substring(29);
          altCorrection = sentAltCorrect.toInt();

          if(DO_WRITE_TO_FLASH){
            lastCommand = "SETALTCORRECTION";
            Serial2.println("WARNING: ABOUT TO WRITE TO FLASH: ALT CORRECTION");
            altCorrectionFlash.write(altCorrection);
          }
          
        }else if(stringVersionReceivedChars == "CMD,2617,CX,CLEARFLASH"){
          Serial2.println("received command to clear flash");
            altCorrection = 0;
            flightStage = 0;
            if(DO_WRITE_TO_FLASH){
              lastCommand = "CLEARFLASH";
              Serial2.println("WARNING: ABOUT TO RESET FLASH VALUES");
              // clear values
              flightStageFlash.write(0);
              altCorrectionFlash.write(0);
            }
              
        }else if(stringVersionReceivedChars == "CMD,2617,CX,ON"){
              Serial2.println("recieved on command");
              lastCommand = "CXON";
              doSendData = 1;
              
        }else if(stringVersionReceivedChars == "CMD,2617,CX,OFF"){
          Serial2.println("recieved off command");
          lastCommand = "CXOFF";
          doSendData = 0;
          
        }else if(stringVersionReceivedChars == "CMD,2617,CX,ZEROSERVOS"){
          Serial2.println("recieved zero servos command");
          lastCommand = "ZEROSERVOS";
          myServo2.attach(9); //pin 9 arduino is pin 12 samd
          myServo2.write(0);
          delay(100);
          myServo2.detach();
          myServo1.attach(15); //pin 15 arduino is pin 7 samd
          myServo1.write(0);
          delay(100);
          myServo1.detach();
        }
        
        newData = false;
    }
}

void recvWithStartEndMarkers2() {
    static boolean recvInProgress2 = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
    
    while (Serial3.available() > 0 && newData2 == false) {
        rc = Serial3.read();

        if (recvInProgress2 == true) {
            if (rc != endMarker) {
                receivedChars2[ndx] = rc;
                ndx++;
                if (ndx >= numChars2) {
                    ndx = numChars2 - 1;
                }
            }
            else {
                receivedChars2[ndx] = '\0'; // terminate the string
                recvInProgress2 = false;
                ndx = 0;
                newData2 = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress2 = true;
        }
    }
}

void showNewData2() {
  if (newData2 == true) {
        String stringVersionReceivedChars;
        stringVersionReceivedChars = receivedChars2;
        /*if(stringVersionReceivedChars == "CMD,2617,CX,PING"){
          Serial2.println("CMD_2617_CX_PING");
          lastCommand = "PING";          */
        Serial2.println(stringVersionReceivedChars);
        
        newData2 = false;
    }
}

/*
void writePayloadXBee(String packet){

  int packetLength = packet.length();

  //calculate chexum, get packet in bytes
  long chexum = 0x10 + 0x01 + 0x13 + 0xa2 + 0x41 + 0xba + 0x07 + 0x85 + 0xff + 0xfe;
  byte buffer[packet.length() + 1];
  packet.getBytes(buffer, packet.length() + 1);

  // strings have extra 0 at end
  for (int i=0; i < packet.length(); i++){
      chexum += buffer[i];
  }
  
  Serial3.write(0x7e); //Start delimiter
  Serial3.write((byte)0x0); //Length
  Serial3.write(packetLength + 14); //Length
  Serial3.write(0x10); //Frame type
  Serial3.write(0x01); //Frame ID
  //64-bit address
  Serial3.write((byte)0x0);
  Serial3.write(0x13);
  Serial3.write(0xa2);
  Serial3.write((byte)0x0);
  Serial3.write(0x41);
  Serial3.write(0xba);
  Serial3.write(0x07);
  Serial3.write(0x85);
  //16-bit address - reserved 
  Serial3.write(0xff);
  Serial3.write(0xfe);
  Serial3.write((byte)0x0);//broadcast radius
  Serial3.write((byte)0x0); //transmit options
  //Data & Checksum
   for (int i=0; i < packet.length(); i++){
    Serial3.write(buffer[i]);
   }
  //Checksum
  Serial3.write(0xff - (chexum & 0xff));
}

void readPayloadXBee(){
  String incomingData = "";
  
  if (Serial3.available() >= 16){
    //Serial2.println("serial 3 avail");
    //while(Serial3.available() > 0){
      //Serial2.println(String(Serial3.read()));
    //}
    
    if(Serial3.read() == 0x7E){
      for (int i = 1; i<15; i++){
        byte discardByte = Serial3.read();
      }
      while(Serial3.available() > 0){
        char dataIn = Serial3.read();
        //if (dataIn == 0x7E){
          //break;
        //}
        //else {
          incomingData += dataIn; 
        //}
      }
      incomingData.remove(incomingData.length()-1);
    }
  } 
  if (incomingData != ""){
    Serial2.println(incomingData);
  }
}
*/
