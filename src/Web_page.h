const char index_html[] PROGMEM = R"rawliteral(
<!doctype html>
<html>
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0">
<title>RoArm-M3</title>
<style type="text/css">
    
    body {
        background-image: linear-gradient(#3F424F, #1E212E);
        background-image: -o-linear-gradient(#3F424F, #1E212E);
        background-image: -moz-linear-gradient(#3F424F, #1E212E);
        background-image: -webkit-linear-gradient(#3F424F, #1E212E);
        background-image: -ms-linear-gradient(#3F424F, #1E212E);
        font-family: "roboto",helt "sans-serif";
        font-weight: lighter;
        color: rgba(216,216,216,0.8);
        background-size: cover;
        background-position: center 0;
        background-attachment: fixed;
        color: rgba(255,255,255,0.6);
        border: 0px;
        margin: 0;
        padding: 0;
        font-size: 14px;
    }
    main{
        width: 516px;
        margin: auto;
        margin-bottom: 80px;
    }
    section > div{
        width: 516px;
    }
    button {
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
    }
    .btn-all, .btn-of, .btn-num, .btn-num-lr, .servo-btn-num, .btn-stop, .btn-set{cursor: pointer}
    .tittle-h2{
        text-align: center;
        font-weight: normal;
        font-size: 1.8em;
        color: rgba(255,255,255,0.8);
        text-transform: uppercase;
    }
    .set-btn-frame{
        display: flex;
        justify-content: space-between;
        align-items: flex-end;
    }
    .set-btn-frame-i{
        width: 360px;
        display: inline-flex;
        justify-content: space-between;
        text-align: center;
    }
    .set-btn-sections{width: 48px;}
    .btn-num{
        width: 48px;
        height: 48px;
        margin: 1px 0 0 0;
        font-size: 14px;
        font-weight: lighter;
    }
    .btn-num-lr{
        width: 48px;
        height: 46px;
        font-size: 14px;
    }
    .btn-num-up{border-radius: 4px 4px 0 0}
    .btn-num-down{border-radius: 0 0 4px 4px}
    .btn-num-bg{
        background-color: rgba(255,255,255,0.06);
        border: none;
        color: rgba(255,255,255,0.5);
    }
    .btn-num-bg:hover{background-color: rgba(255,255,255,0.02);}
    .btn-all, .btn-of{
        background-color: rgba(164,169,186,0.25);
        border: none;
        border-radius: 4px;
        font-size: 14px;
        color: rgba(255,255,255,0.5);
        font-weight: lighter;
    }
    .btn-of-active{
        background-color: rgba(38,152,234,0.1);
        color: #1EA1FF;
        border: 1px solid #1EA1FF;
        border-radius: 4px;
        font-size: 14px;
        
    }
    .btn-all:hover, .btn-of:hover{background-color:rgba(164,169,186,0.15);}
    .btn-main-l{
        width: 126px;
        height: 97px;
    }
    .init-posit{
        vertical-align: bottom;
    }
    .record-mum-set > div {
        display: flex;
        justify-content: space-between;
        margin-top: 54px;
    }
    .record-mum-set > div > div:last-child {
        width: 320px;
        display: flex;
        justify-content: space-between;
        align-items: flex-start;
    }
    .record-mum-set > div > div:last-child > div {
        display: flex;
        width: 164px;
        border: 1px solid rgba(206,206,206,0.15);
        border-radius: 4px;
    }
    .num-insert{
        background-color: rgba(0,0,0,0.01);
        border: 0px;
        width: 68px;
        text-align: center;
        color: rgba(255,255,255,0.8);
        font-size: 14px;
        font-weight: lighter;
    }
    .btn-main-s{
        width: 126px;
        height: 48px;
    }
    .two-btn > div{
        display: flex;
        justify-content: space-between;
        margin: 10px 0;
    }
    .btn-main-m{
        width: 49%;
        height: 48px;
    }
    .two-btn1{
        border-top:1px dashed rgba(216,216,216,0.24) ;
        border-bottom:1px dashed rgba(216,216,216,0.24) ;
        padding: 30px 0;
        margin: 30px 0;
    }
    .Servo-set > div{
        display: flex;
        justify-content: space-between;
    }
    .Servo-set > P{
        font-size: 1.5em;
        text-align: center;
    }
    .servo-btn-num{
        height: 48px;
        width: 126px;
        border-radius: 4px;
        font-size: 14px;
        font-weight: lighter;
    }
    .servo-btn-num + button{margin-left: 30px;}
    .Servo-set{margin: 30px 0;}
    .sec-5{
        position: fixed;
        bottom: 0px;
        width: 100%;
        display: flex;
        justify-content: center;
        padding: 40px 0;
        background-image: linear-gradient(rgba(30,33,46,0),rgba(30,33,46,1));
        
    }
    .sec-5 button{margin: 0 14px;}
    .btn-stop{
        width: 204px;
        height: 48px;
        background-color: rgba(181,104,108,1);
        color: white;
        border-radius: 1000px;
        border: none;
    }
    .btn-set{
        width: 48px;
        height: 48px;
        background-color: rgba(115,134,151,1);
        border-radius: 1000px;
        border: none;
    }
    .btn-set:hover{background-color: rgba(115,134,151,0.5);}
    .record-tt{
        color: rgba(255,255,255,0.5);
        font-size: 14px;
        
    }
    .record-height{height: 1px;}
    .sec-infotext{
        font-size: 14px;
        text-align: center;
        color: rgba(255,255,255,0.4)
    }
    .btn-stop:hover{background-color: rgba(181,104,108,0.5);}
    input::-webkit-outer-spin-button,input::-webkit-inner-spin-button {
        -webkit-appearance: none;
    }
    input[type='number']{
        -moz-appearance: textfield;
    }
    .sec-infotext p{word-break:break-all;}
    .feedb-p textarea{
        width: 100%;
        height: 80px;
        padding: 10px;
        background-color: rgba(0,0,0,0);
        border: 1px solid rgba(194,196,201,0.15);
        border-radius: 4px;
        color: rgba(255, 255, 255, 0.8);
        font-size: 1.2em;
        resize: vertical;
        margin-bottom: 10px;
    }
    .feedb-p > div {
        display: flex;
        justify-content: center;
    }
    .info-box{
        /* border: 1px solid rgba(194,196,201,0.15); */
        padding: 10px;
        margin: 10px 0;
        border-radius: 4px;
        background-color: rgba(255,255,255,0.06);
        border: none;
        color: rgba(255,255,255,0.5);
    }
    .info-box p{
        margin: 0;
        word-break: break-all;
    }
    .info-box > div{margin-right: 20px;}
    .json-cmd-info{cursor: pointer;}
    .json-cmd-info:hover{background-color: rgba(255,255,255,0.02);}
    .w-btn{
        background-color: rgba(0,0,0,0);
        border: 0;
        color: inherit;
    }
    .cmd-value{color: rgba(255,255,255,0.8);}
    @media screen and (min-width: 768px) and (max-width: 1200px){
        main{
            width: 516px;
            display: block;
            margin-bottom: 150px;
        }
        main section{
            margin-bottom: 30px;
        }
        .record-mum-set > div{margin-top: 30px;}
        .sec-2{padding-bottom: 30px;}
    }
    @media screen and (min-width: 360px) and (max-width: 767px){
        main{
            width: 94vw;
            display: block;
        }
        section > div{width: auto;}
        .set-btn-frame{
            display: block;
        }
        .set-btn-frame-i{width: 100%;}
        .btn-main-l{
            width: 100%;
            height: 48px;
        }
        .init-posit{
            margin-top: 30px;
        }
        .record-mum-set > div{display: block;}
        .record-mum-set > div > div:last-child{
            width: 100%;
        }
        .sec-5 button{
            margin: 0 4px;
        }
        .two-btn button:first-child{margin-right: 10px;}
        .servo-btn-num{
            flex: 1;
        }
        .btn-main-s{width: 33.333%;}
        .servo-btn-num + button{margin-left: 10px;}
        .btn-main-m{
            flex: 1;
            width: auto;
        }
        .record-mum-set > div{margin:  30px 0}
        main section{
            margin-bottom: 30px;
        }
        .record-mum-set > div{margin-top: 30px;}
        .record-height{display: none;}
    }
</style>
</head>

<body>
    <main>
        <div>
            <section>
                <div>
                    <div>
                        <h2 class="tittle-h2">AngleCtrl</h2>
                        <div class="set-btn-frame">
                            <div class="set-btn-frame-i">
                                <div class="set-btn-sections">
                                    <p id="anglectrl-t1">100</p>
                                    <div>
                                        <button class="btn-num btn-num-bg btn-num-up" onmousedown="cmdSend(0,1,1);" ontouchstart="cmdSend(0,1,1);" onmouseup="cmdSend(0,1,0);" ontouchend="cmdSend(0,1,0)">B L</button>
                                        <button class="btn-num btn-num-bg btn-num-down" onmousedown="cmdSend(0,1,2);" ontouchstart="cmdSend(0,1,2);" onmouseup="cmdSend(0,1,0);" ontouchend="cmdSend(0,1,0)">B R</button>
                                    </div>
                                </div>
                                <div class="set-btn-sections">
                                    <p id="anglectrl-t2">100</p>
                                    <div>
                                        <button class="btn-num btn-num-bg btn-num-up" onmousedown="cmdSend(0,2,1);" ontouchstart="cmdSend(0,2,1);" onmouseup="cmdSend(0,2,0);" ontouchend="cmdSend(0,2,0)">S D</button>
                                        <button class="btn-num btn-num-bg btn-num-down" onmousedown="cmdSend(0,2,2);" ontouchstart="cmdSend(0,2,2);" onmouseup="cmdSend(0,2,0);" ontouchend="cmdSend(0,2,0)">S U</button>
                                    </div>
                                </div>
                                <div class="set-btn-sections">
                                    <p id="anglectrl-t3">100</p>
                                    <div>
                                        <button class="btn-num btn-num-bg btn-num-up" onmousedown="cmdSend(0,3,1);" ontouchstart="cmdSend(0,3,1);" onmouseup="cmdSend(0,3,0);" ontouchend="cmdSend(0,3,0)">E D</button>
                                        <button class="btn-num btn-num-bg btn-num-down" onmousedown="cmdSend(0,3,2);" ontouchstart="cmdSend(0,3,2);" onmouseup="cmdSend(0,3,0);" ontouchend="cmdSend(0,3,0)">E U</button>
                                    </div>
                                </div>
                                <div class="set-btn-sections">
                                    <p id="anglectrl-t4">100</p>
                                    <div>
                                        <button class="btn-num btn-num-bg btn-num-up" onmousedown="cmdSend(0,4,1);" ontouchstart="cmdSend(0,4,1);" onmouseup="cmdSend(0,4,0);" ontouchend="cmdSend(0,4,0)">W+ DG</button>
                                        <button class="btn-num btn-num-bg btn-num-down" onmousedown="cmdSend(0,4,2);" ontouchstart="cmdSend(0,4,2);" onmouseup="cmdSend(0,4,0);" ontouchend="cmdSend(0,4,0)">W- UG</button>
                                    </div>
                                </div>
                                <div class="set-btn-sections">
                                    <p id="anglectrl-t5">100</p>
                                    <div>
                                        <button class="btn-num btn-num-bg btn-num-up" onmousedown="cmdSend(0,5,1);" ontouchstart="cmdSend(0,5,1);" onmouseup="cmdSend(0,5,0);" ontouchend="cmdSend(0,5,0)">R+ DG</button>
                                        <button class="btn-num btn-num-bg btn-num-down" onmousedown="cmdSend(0,5,2);" ontouchstart="cmdSend(0,5,2);" onmouseup="cmdSend(0,5,0);" ontouchend="cmdSend(0,5,0)">R- UG</button>
                                    </div>
                                </div>
                                <div class="set-btn-sections">
                                    <p id="anglectrl-t6">100</p>
                                    <div>
                                        <button class="btn-num btn-num-bg btn-num-up" onmousedown="cmdSend(0,6,1);" ontouchstart="cmdSend(0,6,1);" onmouseup="cmdSend(0,6,0);" ontouchend="cmdSend(0,6,0)">G+ DG</button>
                                        <button class="btn-num btn-num-bg btn-num-down" onmousedown="cmdSend(0,6,2);" ontouchstart="cmdSend(0,6,2);" onmouseup="cmdSend(0,6,0);" ontouchend="cmdSend(0,6,0)">G- UG</button>
                                    </div>
                                </div>
                            </div>
                            <div class="init-posit">
                                <button class="btn-all btn-main-l btn-all-bg" onclick="cmdInit();">INIT</button>
                            </div>
                        </div>
                    </div>
                    <div class="two-btn">
                        <div>
                            <button class="btn-of btn-main-m btn-all-bg" onclick="torqueCmd(0);">Torque OFF</button>
                            <button class="btn-of btn-main-m btn-all-bg" onclick="torqueCmd(1);">Torque ON</button>
                        </div>
                        <div>
                            <button class="btn-of btn-main-m btn-all-bg" onclick="dynamicCmd(0);">DEFA OFF</button>
                            <button class="btn-of btn-main-m btn-all-bg" onclick="dynamicCmd(1);">DEFA ON</button>
                        </div>
                        <div>
                            <button class="btn-of btn-main-m btn-all-bg" onclick="ledCmd(0);">LED OFF</button>
                            <button class="btn-of btn-main-m btn-all-bg" onclick="ledCmd(255);">LED ON</button>
                        </div>
                        <div>
                            <button class="btn-of btn-main-m btn-all-bg" onclick="window.location.href='/horiDrag'">HORIZONTAL DRAG</button>
                            <button class="btn-of btn-main-m btn-all-bg" onclick="window.location.href='/vertDrag'">VERTICAL DRAG</button>
                        </div>
                    </div>
                    <div>
                        <h2 class="tittle-h2">CoordCtrl</h2>
                        <div class="set-btn-frame">
                            <div class="set-btn-frame-i">
                                <div class="set-btn-sections">
                                    <p id="coordCtrl-t1">100</p>
                                    <div>
                                        <button class="btn-num btn-num-bg btn-num-up" onmousedown="cmdSend(1,1,1);" ontouchstart="cmdSend(1,1,1);" onmouseup="cmdSend(1,1,0);" ontouchend="cmdSend(1,1,0)">X +</button>
                                        <button class="btn-num btn-num-bg btn-num-down" onmousedown="cmdSend(1,1,2);" ontouchstart="cmdSend(1,1,2);" onmouseup="cmdSend(1,1,0);" ontouchend="cmdSend(1,1,0)">X -</button>
                                    </div>
                                </div>
                                <div class="set-btn-sections">
                                    <p id="coordCtrl-t2">100</p>
                                    <div>
                                        <button class="btn-num btn-num-bg btn-num-up" onmousedown="cmdSend(1,2,1);" ontouchstart="cmdSend(1,2,1);" onmouseup="cmdSend(1,2,0);" ontouchend="cmdSend(1,2,0)">Y +</button>
                                        <button class="btn-num btn-num-bg btn-num-down" onmousedown="cmdSend(1,2,2);" ontouchstart="cmdSend(1,2,2);" onmouseup="cmdSend(1,2,0);" ontouchend="cmdSend(1,2,0)">Y -</button>
                                    </div>
                                </div>
                                <div class="set-btn-sections">
                                    <p id="coordCtrl-t3">100</p>
                                    <div>
                                        <button class="btn-num btn-num-bg btn-num-up" onmousedown="cmdSend(1,3,1);" ontouchstart="cmdSend(1,3,1);" onmouseup="cmdSend(1,3,0);" ontouchend="cmdSend(1,3,0)">Z +</button>
                                        <button class="btn-num btn-num-bg btn-num-down" onmousedown="cmdSend(1,3,2);" ontouchstart="cmdSend(1,3,2);" onmouseup="cmdSend(1,3,0);" ontouchend="cmdSend(1,3,0)">Z -</button>
                                    </div>
                                </div>
                                <div class="set-btn-sections">
                                    <p id="coordCtrl-t4">100</p>
                                    <div>
                                        <button class="btn-num btn-num-bg btn-num-up" onmousedown="cmdSend(1,4,1);" ontouchstart="cmdSend(1,4,1);" onmouseup="cmdSend(1,4,0);" ontouchend="cmdSend(1,4,0)">T +</button>
                                        <button class="btn-num btn-num-bg btn-num-down" onmousedown="cmdSend(1,4,2);" ontouchstart="cmdSend(1,4,2);" onmouseup="cmdSend(1,4,0);" ontouchend="cmdSend(1,4,0)">T -</button>
                                    </div>
                                </div>
                                <div class="set-btn-sections">
                                    <p id="coordCtrl-t5">100</p>
                                    <div>
                                        <button class="btn-num btn-num-bg btn-num-up" onmousedown="cmdSend(1,5,1);" ontouchstart="cmdSend(1,5,1);" onmouseup="cmdSend(1,5,0);" ontouchend="cmdSend(1,5,0)">R +</button>
                                        <button class="btn-num btn-num-bg btn-num-down" onmousedown="cmdSend(1,5,2);" ontouchstart="cmdSend(1,5,2);" onmouseup="cmdSend(1,5,0);" ontouchend="cmdSend(1,5,0)">R -</button>
                                    </div>
                                </div>
                                <div class="set-btn-sections">
                                    <p id="coordCtrl-t6">100</p>
                                    <div>
                                        <button class="btn-num btn-num-bg btn-num-up" onmousedown="cmdSend(1,6,1);" ontouchstart="cmdSend(1,6,1);" onmouseup="cmdSend(1,6,0);" ontouchend="cmdSend(1,6,0)">G +</button>
                                        <button class="btn-num btn-num-bg btn-num-down" onmousedown="cmdSend(1,6,2);" ontouchstart="cmdSend(1,6,2);" onmouseup="cmdSend(1,6,0);" ontouchend="cmdSend(1,6,0)">G -</button>
                                    </div>
                                </div>
                            </div>
                            <div class="init-posit">
                                <button class="btn-all btn-main-l btn-all-bg" onclick="cmdInit();">INIT</button>
                            </div>
                        </div>
                    </div>
                </div>
            </section>
            <section>
                <div>
                    <div class="fb-input-info">
                        <h2 class="tittle-h2">Feedback infomation</h2>
                        <div class="sec-infotext">
                            <p id="GetInfoText">Json feedback infomation shows here.</p>
                        </div>
                        <div class="feedb-p">
                            <div>
                                <textarea id="jsonData" placeholder="Input json cmd here." rows="4"></textarea>
                            </div>
                            <div><button class="btn-of btn-main-m btn-all-bg" onclick="jsonSend();">SEND</button></div>
                        </div>
                        <div class="Servo-set">
                            <p>WIFI SETTINGS</p>

                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_WIFI_ON_BOOT</p>
                                    <p class="cmd-value">{"T":401,"cmd":3}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SET_AP</p>
                                    <p class="cmd-value">{"T":402,"ssid":"RoArm-M3","password":"12345678"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SET_STA</p>
                                    <p class="cmd-value">{"T":403,"ssid":"yourWifi","password":"yourPassword"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_WIFI_APSTA</p>
                                    <p class="cmd-value">{"T":404,"ap_ssid":"RoArm-M3","ap_password":"12345678","sta_ssid":"yourWifi","sta_password":"yourPassword"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_WIFI_INFO</p>
                                    <p class="cmd-value">{"T":405}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_WIFI_CONFIG_CREATE_BY_STATUS</p>
                                    <p class="cmd-value">{"T":406}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_WIFI_CONFIG_CREATE_BY_INPUT</p>
                                    <p class="cmd-value">{"T":407,"mode":3,"ap_ssid":"RoArm-M3","ap_password":"12345678","sta_ssid":"yourWifi","sta_password":"yourPassword"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>

                            <p>ESP-NOW SETTINGS</p>

                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_BROADCAST_FOLLOWER</p>
                                    <p class="cmd-value">{"T":300,"mode":0,"mac":"CC:DB:A7:5B:E4:1C"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_ESP_NOW_CONFIG</p>
                                    <p class="cmd-value">{"T":301,"mode":0,"dev":0,"cmd":0,"megs":0}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_GET_MAC_ADDRESS</p>
                                    <p class="cmd-value">{"T":302}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_ESP_NOW_ADD_FOLLOWER</p>
                                    <p class="cmd-value">{"T":303,"mac":"CC:DB:A7:5B:E4:1C"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_ESP_NOW_REMOVE_FOLLOWER</p>
                                    <p class="cmd-value">{"T":304,"mac":"CC:DB:A7:5B:E4:1C"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_ESP_NOW_MANY_CTRL</p>
                                    <p class="cmd-value">{"T":305,"dev":0,"b":0,"s":0,"e":1.57,"t":0,"r":0,"h":1.57,"cmd":0,"megs":"hello!"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_ESP_NOW_SINGLE</p>
                                    <p class="cmd-value">{"T":306,"mac":"FF:FF:FF:FF:FF:FF","dev":0,"b":0,"s":0,"e":1.57,"t":0,"r":0,"h":1.57,"cmd":0,"megs":"hello!"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>

                            <p>TORQUE CTRL</p>

                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_TORQUE_CTRL</p>
                                    <p class="cmd-value">{"T":210,"cmd":0}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>

                            <p>DYNAMIC ADAPTATION</p>

                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SET_NEW_X</p>
                                    <p class="cmd-value">{"T":112,"mode":1,"b":60,"s":110,"e":50,"t":50,"r":50,"h":50}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>

                            <p>MOVING CTRL</p>

                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_MOVE_INIT</p>
                                    <p class="cmd-value">{"T":100}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SINGLE_JOINT_CTRL</p>
                                    <p class="cmd-value">{"T":101,"joint":0,"rad":0,"spd":0,"acc":10}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_JOINTS_RAD_CTRL</p>
                                    <p class="cmd-value">{"T":102,"base":0,"shoulder":0,"elbow":1.57,"wrist":0,"roll":0,"hand":1.57,"spd":0,"acc":10}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_XYZT_GOAL_CTRL</p>
                                    <p class="cmd-value">{"T":104,"x":235,"y":0,"z":234,"t":0,"r":0,"g":3.14,"spd":0.25}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_XYZT_DIRECT_CTRL</p>
                                    <p class="cmd-value">{"T":1041,"x":235,"y":0,"z":234,"t":0,"r":0,"g":3.14}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SERVO_RAD_FEEDBACK</p>
                                    <p class="cmd-value">{"T":105}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_EOAT_HAND_CTRL</p>
                                    <p class="cmd-value">{"T":106,"cmd":3.14,"spd":0,"acc":0}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SINGLE_JOINT_ANGLE</p>
                                    <p class="cmd-value">{"T":121,"joint":1,"angle":0,"spd":10,"acc":10}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_JOINTS_ANGLE_CTRL</p>
                                    <p class="cmd-value">{"T":122,"b":0,"s":0,"e":90,"t":0,"r":0,"h":180,"spd":10,"acc":10}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_CONSTANT_CTRL</p>
                                    <p class="cmd-value">{"T":123,"m":0,"axis":0,"cmd":0,"spd":3}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_DELAY_MILLIS</p>
                                    <p class="cmd-value">{"T":111,"cmd":3000}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>

                            <p>EOAT CTRL</p>

                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_EOAT_GRAB_TORQUE</p>
                                    <p class="cmd-value">{"T":107,"tor":200}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>

                            <p>JOINTS PID CTRL</p>

                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SET_JOINT_PID</p>
                                    <p class="cmd-value">{"T":108,"joint":3,"p":16,"i":0}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_RESET_PID</p>
                                    <p class="cmd-value">{"T":109}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>

                            <p>SET X-AXIS</p>

                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SET_NEW_X</p>
                                    <p class="cmd-value">{"T":110,"xAxisAngle":0}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>

                            <p>MISSION & STEPS EDIT</p>

                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_CREATE_MISSION</p>
                                    <p class="cmd-value">{"T":220,"name":"mission_a","intro":"test mission created in flash."}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_MISSION_CONTENT</p>
                                    <p class="cmd-value">{"T":221,"name":"mission_a"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_APPEND_STEP_JSON</p>
                                    <p class="cmd-value">{"T":222,"name":"mission_a","step":"{\"T\":104,\"x\":235,\"y\":0,\"z\":234,\"t\":3.14,\"r\":0,\"g\":0,\"spd\":0.25}"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_APPEND_STEP_FB</p>
                                    <p class="cmd-value">{"T":223,"name":"mission_a","spd":0.25}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_APPEND_DELAY</p>
                                    <p class="cmd-value">{"T":224,"name":"mission_a","delay":3000}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_INSERT_STEP_JSON</p>
                                    <p class="cmd-value">{"T":225,"name":"mission_a","stepNum":3,"step":"{\"T\":114,\"led\":255}"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_INSERT_STEP_FB</p>
                                    <p class="cmd-value">{"T":226,"name":"mission_a","stepNum":3,"spd":0.25}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_INSERT_DELAY</p>
                                    <p class="cmd-value">{"T":227,"stepNum":3,"delay":3000}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_REPLACE_STEP_JSON</p>
                                    <p class="cmd-value">{"T":228,"name":"mission_a","stepNum":3,"step":"{\"T\":114,\"led\":255}"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_REPLACE_STEP_FB</p>
                                    <p class="cmd-value">{"T":229,"name":"mission_a","stepNum":3}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_REPLACE_DELAY</p>
                                    <p class="cmd-value">{"T":230,"name":"mission_a","stepNum":3,"delay":3000}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_DELETE_STEP</p>
                                    <p class="cmd-value">{"T":231,"name":"mission_a","stepNum":3}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_MOVE_TO_STEP</p>
                                    <p class="cmd-value">{"T":241,"name":"mission_a","stepNum":3}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_MISSION_PLAY</p>
                                    <p class="cmd-value">{"T":242,"name":"mission_a","times":3}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>

                            <p>FILE SYSTEM CTRL</p>
                            
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SCAN_FILES</p>
                                    <p class="cmd-value">{"T":200}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_CREATE_FILE</p>
                                    <p class="cmd-value">{"T":201,"name":"file.txt","content":"inputContentHere."}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_READ_FILE</p>
                                    <p class="cmd-value">{"T":202,"name":"file.txt"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_DELETE_FILE</p>
                                    <p class="cmd-value">{"T":203,"name":"file.txt"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_APPEND_LINE</p>
                                    <p class="cmd-value">{"T":204,"name":"file.txt","content":"inputContentHere."}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_INSERT_LINE</p>
                                    <p class="cmd-value">{"T":205,"name":"file.txt","lineNum":3,"content":"content"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_REPLACE_LINE</p>
                                    <p class="cmd-value">{"T":206,"name":"file.txt","lineNum":3,"content":"Content"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_READ_LINE</p>
                                    <p class="cmd-value">{"T":207,"name":"file.txt","lineNum":3}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_DELETE_LINE</p>
                                    <p class="cmd-value">{"T":208,"name":"file.txt","lineNum":3}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>

                            <p>SWITCH CTRL</p>
                            
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SWITCH_CTRL</p>
                                    <p class="cmd-value">{"T":113,"pwm_a":-255,"pwm_b":-255}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_LIGHT_CTRL</p>
                                    <p class="cmd-value">{"T":114,"led":255}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SWITCH_OFF</p>
                                    <p class="cmd-value">{"T":115}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>

                            <p>SERVO SETTINGS</p>

                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SET_SERVO_ID</p>
                                    <p class="cmd-value">{"T":501,"raw":1,"new":11}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SET_MIDDLE</p>
                                    <p class="cmd-value">{"T":502,"id":11}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SET_SERVO_PID</p>
                                    <p class="cmd-value">{"T":503,"id":14,"p":16}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>

                            <p>ESP32 SETTINGS</p>

                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_REBOOT</p>
                                    <p class="cmd-value">{"T":600}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_FREE_FLASH_SPACE</p>
                                    <p class="cmd-value">{"T":601}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_BOOT_MISSION_INFO</p>
                                    <p class="cmd-value">{"T":602}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_RESET_BOOT_MISSION</p>
                                    <p class="cmd-value">{"T":603}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_NVS_CLEAR</p>
                                    <p class="cmd-value">{"T":604}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_INFO_PRINT</p>
                                    <p class="cmd-value">{"T":605,"cmd":1}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                        </div>
                    </div>
                </div>
            </section>
        </div>
    </main>
    <script>
        document.onkeydown = function (event) {
            var e = event || window.event || arguments.callee.caller.arguments[0];
            if (e && e.keyCode == 13) {
                // alert ("Enter down");
                jsonSend();
            }
        }
        document.addEventListener('DOMContentLoaded', function() {
            var jsonData = document.getElementById('jsonData');
            var infoDiv = document.querySelectorAll('.json-cmd-info');
            for (var i = 0; i < infoDiv.length; i++) {
                var element = infoDiv[i];
                element.addEventListener('click', function() {
                    var cmdValue = this.querySelector('.cmd-value');
                    if (cmdValue) {
                    var cmdValueText = cmdValue.textContent;
                    jsonData.value = cmdValueText;
                    }
                });
            }
        });

        function jsonSend() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                  document.getElementById("GetInfoText").innerHTML =
                  this.responseText;
                }
            };
            xhttp.open("GET", "js?json="+document.getElementById('jsonData').value, true);
            xhttp.send();
        }

        function ledCmd(inputCmd) {
            var jsonCmd = {
                "T":114,
                "led":inputCmd
            }
            var jsonString = JSON.stringify(jsonCmd);
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "js?json=" + jsonString, true);
            xhr.send();
            getData();
        }

        function dynamicCmd(inputCmd) {
            var jsonCmd = {
                "T":112,
                "mode":inputCmd,
                "b":60,
                "s":110,
                "e":50,
                "t":50,
                "r":50,
                "h":50
            }
            var jsonString = JSON.stringify(jsonCmd);
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "js?json=" + jsonString, true);
            xhr.send();
            getData();
        }

        function torqueCmd(inputCmd) {
            var jsonCmd = {
                "T":210,
                "cmd":inputCmd
            }
            var jsonString = JSON.stringify(jsonCmd);
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "js?json=" + jsonString, true);
            xhr.send();
            getData();
        }

        function cmdInit() {
            var jsonCmd = {
                "T":102,
                "base":0,
                "shoulder":0,
                "elbow":1.5707965,
                "wrist":0,
                "roll":0,
                "hand":3.1415926,
                "spd":0,
                "acc":10
            }
            var jsonString = JSON.stringify(jsonCmd);
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "js?json=" + jsonString, true);
            xhr.send();
            getData();
        }

        function cmdSend(inputT, inputA, inputB) {
            var jsonCmd = {
                "T": 123,
                "m": inputT,
             "axis": inputA,
              "cmd": inputB,
              "spd": 10
            }
            var jsonString = JSON.stringify(jsonCmd);
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "js?json=" + jsonString, true);
            xhr.send();
            getData();
        }

        function getData() {
            var jsonCmd = {
                "T": 105
            }
            var jsonString = JSON.stringify(jsonCmd);
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                  document.getElementById("GetInfoText").innerHTML = this.responseText;
                  var jsonResponse = JSON.parse(this.responseText);

                  document.getElementById("anglectrl-t1").innerHTML = jsonResponse.b.toFixed(2);;
                  document.getElementById("anglectrl-t2").innerHTML = jsonResponse.s.toFixed(2);;
                  document.getElementById("anglectrl-t3").innerHTML = jsonResponse.e.toFixed(2);;
                  document.getElementById("anglectrl-t4").innerHTML = jsonResponse.t.toFixed(2);;
                  document.getElementById("anglectrl-t5").innerHTML = jsonResponse.r.toFixed(2);;
                  document.getElementById("anglectrl-t6").innerHTML = jsonResponse.g.toFixed(2);;

                  document.getElementById("coordCtrl-t1").innerHTML = jsonResponse.x.toFixed(2);
                  document.getElementById("coordCtrl-t2").innerHTML = jsonResponse.y.toFixed(2);
                  document.getElementById("coordCtrl-t3").innerHTML = jsonResponse.z.toFixed(2);
                  document.getElementById("coordCtrl-t4").innerHTML = jsonResponse.tit.toFixed(2);
                  document.getElementById("coordCtrl-t5").innerHTML = jsonResponse.r.toFixed(2);
                  document.getElementById("coordCtrl-t6").innerHTML = jsonResponse.g.toFixed(2);
                }
            };
            xhttp.open("GET", "js?json=" + jsonString, true);
            xhttp.send();
        }

        function getDevInfo() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                  document.getElementById("GetInfoText").innerHTML = this.responseText;
                }
            };
            xhttp.open("GET", "getDevInfo", true);
            xhttp.send();
        }

        function opera(x, y) {
            var rs = new Number(document.getElementById(x).value);

            if (y) {
                document.getElementById(x).value = rs + 1;
            } else if( rs >0) {
                document.getElementById(x).value = rs - 1;
            }
       }
        function opera1(x, y) {
            var rs = new Number(document.getElementById(x).value);

            if (y) {
                document.getElementById(x).value = rs + 1;
            } else if( rs >-1) {
                document.getElementById(x).value = rs - 1;
            }
       }
        function opera100(x, y) {
            var rs = new Number(document.getElementById(x).value);

            if (y) {
                document.getElementById(x).value = rs + 100;
            } else if( rs >0) {
                document.getElementById(x).value = rs - 100;
            }
       }
    </script>
</body>
</html>
)rawliteral";