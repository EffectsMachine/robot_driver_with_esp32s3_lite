static const char PROGMEM INDEX_HTML[] = R"HTML(
<!doctype html>
<html>
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0">
<title>LYGION ROBOTICS RD-LITE</title>
<style type="text/css">
    
    body {
        background-image: linear-gradient(#0b1320, #1E212E);
        background-image: -o-linear-gradient(#0b1320, #1E212E);
        background-image: -moz-linear-gradient(#0b1320, #1E212E);
        background-image: -webkit-linear-gradient(#0b1320, #1E212E);
        background-image: -ms-linear-gradient(#0b1320, #1E212E);
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
    input[type='number'] {
        appearance: textfield;
        -moz-appearance: textfield;
        -webkit-appearance: none;
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
                    <h2 class="tittle-h2">Device Infomation</h2>
                    <p id="upTime">Disconnected</p>
                    <p id="baudrate">Current Baud Rate: 1000000</p>
                    <p id="MAC">MAC: FF:FF:FF:FF:FF:FF</p>
                    <div style="margin-bottom: 20px;">
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='confirmAndSend({"T":10,"baud":115200}, "Change buadrate to 115200?");'>115200</button>
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='confirmAndSend({"T":10,"baud":500000}, "Change buadrate to 500K?");'>500K</button>
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='confirmAndSend({"T":10,"baud":1000000}, "Change buadrate to 1M?");'>1M</button>
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='confirmAndSend({"T":10,"baud":3000000}, "Change buadrate to 3M?");'>3M</button>
                    </div>
                </div>
                <div>
                    <h2 class="tittle-h2">Wifi Settings</h2>
                    <div style="margin-bottom: 20px;">
                        <label>AP_SSID:</label>
                        <input id="ap_ssid" value="Robot" style="width:80px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                        <label>AP_PWD:</label>
                        <input id="ap_pwd" value="12345678" style="width:80px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                    </div>
                    <p>STA STATUS:</p>
                    <p id="sta_status">Disconnected</p>
                    <div style="margin-bottom: 20px;">
                        <label>STA_SSID:</label>
                        <input id="sta_ssid" style="width:80px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                        <label>STA_PWD:</label>
                        <input id="sta_pwd" type="password" style="width:80px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                    </div>
                    <div>
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='confirmAndSend(setSta(), "Connect to WIFI:" + document.getElementById("sta_ssid").value + "?");'>SET WIFI</button>
                    </div>
                </div>
                <div>
                    <h2 class="tittle-h2">ST/SM Servo Ctrl</h2>
                    <div class="servoCtrl" style="margin: 15px auto; padding: 20px; background-color: #2f3542; border-radius: 10px; text-align: center; width: 90%; max-width: 650px;">
                    <div style="margin-bottom: 15px;">
                        <label>ID:</label>
                        <input id="stsm_id" type="number" class="num-insert" style="width:80px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                        <label>Pos:</label>
                        <input id="stsm_pos" type="number" class="num-insert" style="width:80px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                    </div>
                    <div style="margin-bottom: 20px;">
                        <label>Spd:</label>
                        <input id="stsm_spd" type="number" class="num-insert" style="width:80px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                        <label>Acc:</label>
                        <input id="stsm_acc" type="number" class="num-insert" style="width:80px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                    </div>
                    <div style="margin-bottom: 20px;">
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='sendJsonCmd({"T":14,"id":parseInt(document.getElementById("stsm_id").value || "0", 10),"state":0});'>Release</button>
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='sendJsonCmd({"T":14,"id":parseInt(document.getElementById("stsm_id").value || "0", 10),"state":1});'>Torque</button>
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='sendJsonCmd({"T":15,"id":parseInt(document.getElementById("stsm_id").value || "0", 10)});'>Feedback</button>
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='confirmAndSend({"T":12,"id":parseInt(document.getElementById("stsm_id").value || "0", 10)}, "Calibrate current position as servo center?");'>Calibration</button>
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='if (confirm("Add this cmd to Misson Input?")){stsmAdd();}'>Add</button>
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px; background:red; color:white;" onclick="stsmAction()">Action</button>
                    </div>
                    <div>
                        <label>Change ID:</label>
                        <input id="stsm_new_id" type="number" class="num-insert" style="width:90px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                        <button class="btn-of btn-all" style="min-width:90px; padding:8px 12px; margin:5px;" onclick='if (confirm("Change servo ID?")){stsmChangID();}'>Set</button>
                    </div>
                    <div>
                        <label>Delay(ms):</label>
                        <input id="stsm_delayTime" type="number" class="num-insert" style="width:90px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                        <button class="btn-of btn-all" style="min-width:90px; padding:8px 12px; margin:5px;" onclick='if (confirm("Add delay to Misson Input?")){stsmDelayAdd();}'>Add</button>
                    </div>
                    </div>

                    <h2 class="tittle-h2">HL Servo Ctrl</h2>
                    <div class="servoCtrl" style="margin: 15px auto; padding: 20px; background-color: #2f3542; border-radius: 10px; text-align: center; width: 90%; max-width: 650px;">
                    <div style="margin-bottom: 15px;">
                        <label> ID:</label>
                        <input id="hl_id" type="number" class="num-insert" style="width:80px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                        <label>Pos:</label>
                        <input id="hl_pos" type="number" class="num-insert" style="width:80px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                    </div>
                    <div style="margin-bottom: 20px;">
                        <label>Spd:</label>
                        <input id="hl_spd" type="number" class="num-insert" style="width:80px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                        <label>Acc:</label>
                        <input id="hl_acc" type="number" class="num-insert" style="width:80px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                    </div>
                    <div style="margin-bottom: 20px;">
                        <label>Current Limit:</label>
                        <input id="hl_cl" type="number" class="num-insert" style="width:80px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                    </div>
                    <div style="margin-bottom: 20px;">
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='sendJsonCmd({"T":24,"id":parseInt(document.getElementById("hl_id").value || "0", 10),"state":0});'>Release</button>
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='sendJsonCmd({"T":24,"id":parseInt(document.getElementById("hl_id").value || "0", 10),"state":1});'>Torque</button>
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='sendJsonCmd({"T":25,"id":parseInt(document.getElementById("hl_id").value || "0", 10)});'>Feedback</button>
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='confirmAndSend({"T":22,"id":parseInt(document.getElementById("hl_id").value || "0", 10)}, "Calibrate current position as servo center?");'>Calibration</button>
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='if (confirm("Add this cmd to Misson Input?")){hlAdd();}'>Add</button>
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px; background:red; color:white;" onclick="hlAction()">Action</button>
                    </div>
                    <div>
                        <label>Change ID:</label>
                        <input id="hl_new_id" type="number" class="num-insert" style="width:90px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                        <button class="btn-of btn-all" style="min-width:90px; padding:8px 12px; margin:5px;" onclick='if (confirm("Change servo ID?")){hlChangID();}'>Set</button>
                    </div>
                    <div>
                        <label>Delay(ms):</label>
                        <input id="hl_delayTime" type="number" class="num-insert" style="width:90px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                        <button class="btn-of btn-all" style="min-width:90px; padding:8px 12px; margin:5px;" onclick='if (confirm("Add delay to Misson Input?")){hlDelayAdd();}'>Add</button>
                    </div>
                    </div>

                    <h2 class="tittle-h2">SC Servo Ctrl</h2>
                    <div class="servoCtrl" style="margin: 15px auto; padding: 20px; background-color: #2f3542; border-radius: 10px; text-align: center; width: 90%; max-width: 650px;">
                    <div style="margin-bottom: 15px;">
                        <label> ID:</label>
                        <input id="sc_id" type="number" class="num-insert" style="width:80px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                        <label>Pos:</label>
                        <input id="sc_pos" type="number" class="num-insert" style="width:80px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                    </div>
                    <div style="margin-bottom: 20px;">
                        <label>Time:</label>
                        <input id="sc_time" type="number" class="num-insert" style="width:80px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                        <label>Spd:</label>
                        <input id="sc_spd" type="number" class="num-insert" style="width:80px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                    </div>
                    <div style="margin-bottom: 20px;">
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='sendJsonCmd({"T":34,"id":parseInt(document.getElementById("sc_id").value || "0", 10),"state":0});'>Release</button>
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='sendJsonCmd({"T":34,"id":parseInt(document.getElementById("sc_id").value || "0", 10),"state":1});'>Torque</button>
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='sendJsonCmd({"T":35,"id":parseInt(document.getElementById("sc_id").value || "0", 10)});'>Feedback</button>
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='if (confirm("Add this cmd to Misson Input?")){scAdd();}'>Add</button>
                        <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px; background:red; color:white;" onclick="scAction()">Action</button>
                    </div>
                    <div>
                        <label>Change ID:</label>
                        <input id="sc_new_id" type="number" class="num-insert" style="width:90px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                        <button class="btn-of btn-all" style="min-width:90px; padding:8px 12px; margin:5px;" onclick='if (confirm("Change servo ID?")){scChangID();}'>Set</button>
                    </div>
                    <div>
                        <label>Delay(ms):</label>
                        <input id="sc_delayTime" type="number" class="num-insert" style="width:90px; margin: 0 12px; padding:6px; border-radius:6px; border:1px solid #555; background:#f1f2f6; color:#2f3542;" />
                        <button class="btn-of btn-all" style="min-width:90px; padding:8px 12px; margin:5px;" onclick='if (confirm("Add delay to Misson Input?")){scDelayAdd();}'>Add</button>
                    </div>
                    </div>
                </div>
            </section>
            <section>
                <div class="fb-input-info">
                        <h2 class="tittle-h2">Automation scripts</h2>
                        <div class="sec-infotext">
                            <p id="AutoScropts">Mission upload status.</p>
                        </div>
                        <div class="feedb-p">
                            <div>
                                <textarea id="jsonAuto" placeholder="Input json cmd here." rows="4"></textarea>
                            </div>
                            <div style="margin-bottom: 20px;">
                                <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='if (confirm("Creat a mission:up?")){createMission("up");}'>Up</button>
                                <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='if (confirm("Creat a mission:down?")){createMission("down");}'>Down</button>
                                <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='if (confirm("Creat a mission:left?")){createMission("left");}'>Left</button>
                                <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='if (confirm("Creat a mission:right?")){createMission("right");}'>Right</button>
                            </div>
                            <div><button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px; background:red; color:white;" onclick='if (confirm("Creat a mission:right?")){createMission("boot_user");}'>On Boot</button></div>
                            <div><button class="btn-of btn-main-m btn-all-bg" onclick="stopMission();">STOP MISSION</button></div>
                        </div>
                </div>
                <div>
                    <div class="fb-input-info">
                        <h2 class="tittle-h2">json interface</h2>
                        <div class="sec-infotext">
                            <p id="GetInfoText">Send single JSON cmd here.</p>
                        </div>
                        <div class="feedb-p">
                            <div>
                                <textarea id="jsonData" placeholder="Input json cmd here." rows="4"></textarea>
                            </div>
                            <div><button class="btn-of btn-main-m btn-all-bg" onclick="jsonSend();">SEND</button></div>
                        </div>
                        <div class="Servo-set">

                            <p>JSON LIST</p>

                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_BREAK_LOOP</p>
                                    <p class="cmd-value">{"T":0}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_WEB_SET_JOINTS_BAUD</p>
                                    <p class="cmd-value">{"T":10,"baud":1000000}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>

                            <p>ST/SM Servo</p>

                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_STSM_CTRL</p>
                                    <p class="cmd-value">{"T":11,"id":1,"pos":2047,"spd":0,"acc":0}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_STSM_SET_MIDDLE</p>
                                    <p class="cmd-value">{"T":12,"id":1}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_STSM_CHANGE_ID</p>
                                    <p class="cmd-value">{"T":13,"old_id":1,"new_id":2}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_STSM_TORQUE_LOCK</p>
                                    <p class="cmd-value">{"T":14,"id":1,"state":1}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_STSM_FEEDBACK</p>
                                    <p class="cmd-value">{"T":15,"id":1}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>

                            <p>HL Servo</p>

                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_HL_CTRL</p>
                                    <p class="cmd-value">{"T":21,"id":1,"pos":2047,"spd":0,"acc":0,"cl":500}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_HL_SET_MIDDLE</p>
                                    <p class="cmd-value">{"T":22,"id":1}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_HL_CHANGE_ID</p>
                                    <p class="cmd-value">{"T":23,"old_id":1,"new_id":2}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_HL_TORQUE_LOCK</p>
                                    <p class="cmd-value">{"T":24,"id":1,"state":1}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_HL_FEEDBACK</p>
                                    <p class="cmd-value">{"T":25,"id":1}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>

                            <p>SC Servo</p>

                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SC_CTRL</p>
                                    <p class="cmd-value">{"T":31,"id":1,"pos":511,"time":0,"spd":0}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SC_CHANGE_ID</p>
                                    <p class="cmd-value">{"T":33,"old_id":1,"new_id":2}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SC_TORQUE_LOCK</p>
                                    <p class="cmd-value">{"T":34,"id":1,"state":1}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SC_FEEDBACK</p>
                                    <p class="cmd-value">{"T":35,"id":1}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>

                            <p>Delay & Display</p>

                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_DELAY</p>
                                    <p class="cmd-value">{"T":51,"delay":1000}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_DISPLAY_SINGLE</p>
                                    <p class="cmd-value">{"T":202,"line":1,"text":"Hello, world!","update":1}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_DISPLAY_UPDATE</p>
                                    <p class="cmd-value">{"T":203}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_DISPLAY_FRAME</p>
                                    <p class="cmd-value">{"T":204,"l1":"Hello!","l2":"world!","l3":"Hello!","l4":"world!"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_DISPLAY_CLEAR</p>
                                    <p class="cmd-value">{"T":205}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_BUZZER_CTRL</p>
                                    <p class="cmd-value">{"T":206,"freq":1000,"duration":1000}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>

                            <p>Wireless Configuration</p>

                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SET_WIFI_MODE</p>
                                    <p class="cmd-value">{"T":400,"mode":1,"ap_ssid":"Robot","ap_password":"12345678","channel":1,"sta_ssid":"ssid","sta_password":"password"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SET_ESP_NOW_MODE</p>
                                    <p class="cmd-value">{"T":411,"mode":1}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_ESP_NOW_SEND</p>
                                    <p class="cmd-value">{"T":413,"mac":"FF,FF,FF,FF,FF,FF","data":"{\"T\":205,\"freq\":500,\"duration\":30}"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_ADD_MAC</p>
                                    <p class="cmd-value">{"T":414,"mac":"FF:FF:FF:FF:FF:FF"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>

                            <p>System</p>

                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_ESP32_REBOOT</p>
                                    <p class="cmd-value">{"T":600}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_CLEAR_NVS</p>
                                    <p class="cmd-value">{"T":601}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_RESET</p>
                                    <p class="cmd-value">{"T":602}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SET_MSG_OUTPUT</p>
                                    <p class="cmd-value">{"T":604,"echo":1,"uart":0,"usb":1}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_SERIAL_FORWARDING</p>
                                    <p class="cmd-value">{"T":605,"sf":1}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>

                            <p>File System</p>

                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_CREATE_MISSION</p>
                                    <p class="cmd-value">{"T":301,"name":"mission1","intro":"introduction for this mission file."}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_APPEND_SETP_JSON</p>
                                    <p class="cmd-value">{"T":303,"name":"boot","json":"{\"T\":205,\"freq\":500,\"duration\":30}"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_INSERT_SETP_JSON</p>
                                    <p class="cmd-value">{"T":304,"name":"boot","step":2,"json":"{\"T\":205,\"freq\":500,\"duration\":30}"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_REPLACE_SETP_JSON</p>
                                    <p class="cmd-value">{"T":305,"name":"boot","step":2,"json":"{\"T\":205,\"freq\":500,\"duration\":30}"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_DELETE_SETP</p>
                                    <p class="cmd-value">{"T":306,"name":"boot","step":2}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_RUN_STEP</p>
                                    <p class="cmd-value">{"T":307,"name":"boot","step":2}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_RUN_MISSION</p>
                                    <p class="cmd-value">{"T":308,"name":"boot","interval":1000,"loop":1}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                            <div class="info-box json-cmd-info">
                                <div>
                                    <p>CMD_DELETE_MISSION</p>
                                    <p class="cmd-value">{"T":309,"name":"boot"}</p>
                                </div>
                                <button class="w-btn">INPUT</button>
                            </div>
                        </div>
                    </div>
                </div>
                <div style="margin-bottom: 20px;">
                    <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='if (confirm("Warning: This will erase all saved WiFi and configuration data. Proceed?")){sendJsonCmd({"T":601});}'>ClearNvs</button>
                    <button class="btn-of btn-all" style="min-width:110px; padding:10px 15px; margin:5px;" onclick='if (confirm("Warning: This will erase all saved mission data. Proceed?")){sendJsonCmd({"T":602});}'>Reset</button>
                </div>
            </section>
        </div>
    </main>
    <script>
        let sock = null;
        let reconnectTimer = null;
        let reconnectDelay = 1000;
        const RECONNECT_MAX = 15000;

        const wsOut = document.getElementById('upTime');

        // auto connection
        window.addEventListener('DOMContentLoaded', () => {
        connectWS();
        });

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
            sendJsonCmd(document.getElementById("jsonData").value);
        }

        function connectWS(manual = false) {
            // ws/wss URL
            const url = (location.protocol === 'https:' ? 'wss://' : 'ws://') + location.host + '/ws';
            if (sock && (sock.readyState === WebSocket.OPEN || sock.readyState === WebSocket.CONNECTING)) {
                return; // connecting or connected
            }
            if (manual) {
                // manual connect
                clearTimeout(reconnectTimer);
                reconnectTimer = null;
                reconnectDelay = 1000;
            }

            wsOut.textContent = 'connecting…';
            sock = new WebSocket(url);


            sock.onopen = () => {
                wsOut.textContent = 'WebSocket Connected';
                // re
                clearTimeout(reconnectTimer);
                reconnectTimer = null;
                reconnectDelay = 1000;
            };

            sock.onmessage = (ev) => {
                try {
                    var jsonResponse = JSON.parse(ev.data);
                    // wsOut.textContent = JSON.stringify(jsonResponse, null, 2);
                    console.log("jsonRecv", jsonResponse);
                    if (jsonResponse.T == 50) {
                        document.getElementById("upTime").innerHTML = 'Uptime: ' + jsonResponse.uptime;
                        document.getElementById("baudrate").innerHTML = 'Current Baud Rate: ' + jsonResponse.baud;
                        document.getElementById("MAC").innerHTML = 'MAC: ' + jsonResponse.mac;
                        document.getElementById("sta_status").innerHTML = jsonResponse.ssid + " IP:" + jsonResponse.sta;
                    } else if (jsonResponse.T == -15) {
                        document.getElementById("stsm_pos").value = jsonResponse.pos;
                        document.getElementById("stsm_spd").value = 0;
                        document.getElementById("stsm_acc").value = 0;
                    } else if (jsonResponse.T == -25) {
                        document.getElementById("hl_pos").value = jsonResponse.pos;
                        document.getElementById("hl_spd").value = 1000;
                        document.getElementById("hl_acc").value = 0;
                        document.getElementById("hl_cl").value = 1000;
                    } else if (jsonResponse.T == -35) {
                        document.getElementById("sc_pos").value = jsonResponse.pos;
                        document.getElementById("sc_spd").value = 0;
                        document.getElementById("sc_time").value = 0;
                    }

                } catch {
                    wsOut.textContent = ev.data; // show raw date if not json
                }
            };

            sock.onerror = () => {
                // for most explorer, it will be 'WebSocket is closed'
            };

            sock.onclose = () => {
                wsOut.textContent = 'Disconnected, reconnecting in' + (reconnectDelay/1000) + 's ...';
                scheduleReconnect();
            };
        }

        function scheduleReconnect() {
            if (reconnectTimer) return;
            reconnectTimer = setTimeout(() => {
                reconnectTimer = null;
                reconnectDelay = Math.min(reconnectDelay * 2, RECONNECT_MAX);
                connectWS();
            }, reconnectDelay);
        }


        function sendJsonCmd(jsonStringOrObject) {
            let payloadObj;

            if (typeof jsonStringOrObject === 'string') {
                if (!jsonStringOrObject.length) {
                alert('NULL JSON'); return;
                }
                try {
                payloadObj = JSON.parse(jsonStringOrObject);
                } catch (e) {
                alert('JSON ERROR：' + e.message); return;
                }
            } else if (typeof jsonStringOrObject === 'object' && jsonStringOrObject !== null) {
                payloadObj = jsonStringOrObject;
            } else {
                alert('WRONG ARGS'); return;
            }

            if (!sock || sock.readyState !== WebSocket.OPEN) {
                alert('WebSocket disconnected'); return;
            }

            sock.send(JSON.stringify(payloadObj));
        }

        function setSta() {
            // {"T":400,"mode":1,"ap_ssid":"LYgion","ap_password":"12345678","channel":1,"sta_ssid":"ssid","sta_password":"password"}
            const jsonData = {
                "T":400,
                "mode":1,
                "ap_ssid":document.getElementById("ap_ssid").value,
                "ap_password":document.getElementById("ap_pwd").value,
                "channel":1,
                "sta_ssid":document.getElementById("sta_ssid").value,
                "sta_password":document.getElementById("sta_pwd").value
            };

            console.log(jsonData);
            sendJsonCmd(jsonData);
        }

        function stsmAction() {
            const jsonData = {
                "T": 11,
                "id": parseInt(document.getElementById("stsm_id").value || "0", 10),
                "pos": parseInt(document.getElementById("stsm_pos").value || "0", 10),
                "spd": parseInt(document.getElementById("stsm_spd").value || "0", 10),
                "acc": parseInt(document.getElementById("stsm_acc").value || "0", 10)
            };

            sendJsonCmd(jsonData);
        }

        const jsonArea = document.getElementById('jsonAuto');
        function stsmAdd() {
            const jsonData = {
                "T": 11,
                "id": parseInt(document.getElementById("stsm_id").value || "0", 10),
                "pos": parseInt(document.getElementById("stsm_pos").value || "0", 10),
                "spd": parseInt(document.getElementById("stsm_spd").value || "0", 10),
                "acc": parseInt(document.getElementById("stsm_acc").value || "0", 10)
            };

            console.log("jsonData", jsonData);

            const jsonStr = JSON.stringify(jsonData);
            if (jsonArea.value === "") {
                jsonArea.value = jsonStr;
            } else {
                jsonArea.value += "\n" + jsonStr;
            }

            jsonArea.scrollTop = jsonArea.scrollHeight;
        }

        function stsmDelayAdd() {
            const jsonData = {
                "T": 51,
                "delay": parseInt(document.getElementById("stsm_delayTime").value || "0", 10)
            };

            console.log("jsonData", jsonData);

            const jsonStr = JSON.stringify(jsonData);
            if (jsonArea.value === "") {
                jsonArea.value = jsonStr;
            } else {
                jsonArea.value += "\n" + jsonStr;
            }

            jsonArea.scrollTop = jsonArea.scrollHeight;
        }

        function stsmChangID() {
            const jsonData = {
                "T": 13,
                "old_id": parseInt(document.getElementById("stsm_id").value || "0", 10),
                "new_id": parseInt(document.getElementById("stsm_new_id").value || "0", 10)
            };
            sendJsonCmd(jsonData);
        }

        // --- --- --- HL --- --- ---

        function hlAction() {
            const jsonData = {
                "T": 21,
                "id": parseInt(document.getElementById("hl_id").value || "0", 10),
                "pos": parseInt(document.getElementById("hl_pos").value || "0", 10),
                "spd": parseInt(document.getElementById("hl_spd").value || "0", 10),
                "acc": parseInt(document.getElementById("hl_acc").value || "0", 10),
                "cl": parseInt(document.getElementById("hl_cl").value || "0", 10)
            };

            sendJsonCmd(jsonData);
        }

        function hlAdd() {
            const jsonData = {
                "T": 21,
                "id": parseInt(document.getElementById("hl_id").value || "0", 10),
                "pos": parseInt(document.getElementById("hl_pos").value || "0", 10),
                "spd": parseInt(document.getElementById("hl_spd").value || "0", 10),
                "acc": parseInt(document.getElementById("hl_acc").value || "0", 10),
                "cl": parseInt(document.getElementById("hl_cl").value || "0", 10)
            };

            console.log("jsonData", jsonData);

            const jsonStr = JSON.stringify(jsonData);
            if (jsonArea.value === "") {
                jsonArea.value = jsonStr;
            } else {
                jsonArea.value += "\n" + jsonStr;
            }

            jsonArea.scrollTop = jsonArea.scrollHeight;
        }

        function hlDelayAdd() {
            const jsonData = {
                "T": 51,
                "delay": parseInt(document.getElementById("hl_delayTime").value || "0", 10)
            };

            console.log("jsonData", jsonData);

            const jsonStr = JSON.stringify(jsonData);
            if (jsonArea.value === "") {
                jsonArea.value = jsonStr;
            } else {
                jsonArea.value += "\n" + jsonStr;
            }

            jsonArea.scrollTop = jsonArea.scrollHeight;
        }

        function hlChangID() {
            const jsonData = {
                "T": 23,
                "old_id": parseInt(document.getElementById("hl_id").value || "0", 10),
                "new_id": parseInt(document.getElementById("hl_new_id").value || "0", 10)
            };
            sendJsonCmd(jsonData);
        }



        // --- --- --- SC --- --- ---

        function scAction() {
            const jsonData = {
                "T": 31,
                "id": parseInt(document.getElementById("sc_id").value || "0", 10),
                "pos": parseInt(document.getElementById("sc_pos").value || "0", 10),
                "time": parseInt(document.getElementById("sc_time").value || "0", 10),
                "spd": parseInt(document.getElementById("sc_spd").value || "0", 10)
            };

            sendJsonCmd(jsonData);
        }

        function scAdd() {
            const jsonData = {
                "T": 31,
                "id": parseInt(document.getElementById("sc_id").value || "0", 10),
                "pos": parseInt(document.getElementById("sc_pos").value || "0", 10),
                "time": parseInt(document.getElementById("sc_time").value || "0", 10),
                "spd": parseInt(document.getElementById("sc_spd").value || "0", 10)
            };

            console.log("jsonData", jsonData);

            const jsonStr = JSON.stringify(jsonData);
            if (jsonArea.value === "") {
                jsonArea.value = jsonStr;
            } else {
                jsonArea.value += "\n" + jsonStr;
            }

            jsonArea.scrollTop = jsonArea.scrollHeight;
        }

        function scDelayAdd() {
            const jsonData = {
                "T": 51,
                "delay": parseInt(document.getElementById("sc_delayTime").value || "0", 10)
            };

            console.log("jsonData", jsonData);

            const jsonStr = JSON.stringify(jsonData);
            if (jsonArea.value === "") {
                jsonArea.value = jsonStr;
            } else {
                jsonArea.value += "\n" + jsonStr;
            }

            jsonArea.scrollTop = jsonArea.scrollHeight;
        }

        function scChangID() {
            const jsonData = {
                "T": 33,
                "old_id": parseInt(document.getElementById("sc_id").value || "0", 10),
                "new_id": parseInt(document.getElementById("sc_new_id").value || "0", 10)
            };
            sendJsonCmd(jsonData);
        }

        async function createMission(missionName) {
            // delete mission
            document.getElementById("AutoScropts").innerHTML = "Deleting mission...";
            const jsonData1 = {
                "T": 309,
                "name": missionName
            };
            sendJsonCmd(jsonData1);
            await new Promise(resolve => setTimeout(resolve, 500));
            // create mission
            document.getElementById("AutoScropts").innerHTML = "Creating mission...";
            const jsonData2 = {
                "T": 301,
                "name": missionName,
                "intro": "a mission created by web"
            };
            sendJsonCmd(jsonData2);
            await new Promise(resolve => setTimeout(resolve, 500));

            const text = document.getElementById("jsonAuto").value;
            const lines = text.split("\n").map(line => line.trim()).filter(line => line !== "");
            const total_num_steps = lines.length;

            for (let [index, line] of lines.entries()) {
                try {
                    const cmd = JSON.parse(line);
                    console.log("cmd " + (index + 1) + " parsed:", cmd);

                    const jsonData3 = {
                        "T": 303,
                        "name": missionName,
                        "json": JSON.stringify(cmd)
                    };
                    sendJsonCmd(jsonData3);
                    console.log(`sending ${index + 1}/${total_num_steps}`);
                    document.getElementById("AutoScropts").innerHTML = `sending ${index + 1}/${total_num_steps}`;
                    // delay 500ms
                    await new Promise(resolve => setTimeout(resolve, 500));
                } catch (err) {
                    console.error("cmd " + (index + 1) + " not a valid JSON:", line);
                    document.getElementById("AutoScropts").innerHTML = "cmd " + (index + 1) + " not a valid JSON:" + line;
                }
            }
            console.log(`mission created.`);
            document.getElementById("AutoScropts").innerHTML = `Mission created.`;
            confirm("Mission created. Press OK to continue.");
        }

        function stopMission() {
            const jsonData = {
                "T": 0
            };
            sendJsonCmd(jsonData);
        }

        function confirmAndSend(data, message) {
            if (confirm(message)) {
                sendJsonCmd(data);
            }
        }
    </script>
</body>
</html>
)HTML";