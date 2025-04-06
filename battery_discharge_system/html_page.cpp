#include <WString.h>

String pageHTML = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Discharger</title>
</head>
<style>
    
    body{
        background-color: #cccccc;
        font-family: Arial, Helvetica, sans-serif;
    }

    @media (max-width:1080px) {
        body{
            font-size: 4.5vw;
        }
    }

    @media (min-width:1081px) {
        body{
            font-size: 30px;
        }
    }

    table{
        margin-top: 3em;
    }

    table td{
        height: 2em;
    }

    .parameter {
        color: blue;
        width: 9em;
    }

    .value{
        color: darkslateblue;
        width: 3em;
    }

    .choice_arrow{
        width: 1em;
        opacity: 0;
    }

    button {
              width: 9em;
              height: 2em;
			  border: none;
			  color: white;
			  border-radius: 1.5em;
              margin-top: 1em;
			  text-align: center;
			  display: inline-block;
              font-size: 1em;
	}
    
    button:active{
        transform: scale(0.98);
        box-shadow: 3px 2px 22px 1px rgba(0, 0, 0, 0.24);
    }

</style>
<body >
    <center>
        <table>
            <caption style="text-align: right;"><font style="color: black; font-weight: 1000; font-size: 1.1em;">Параметры устройства</font></caption>
            <tr>
                <td class="choice_arrow" id="line_0_u"><font>&#9658;</font></td>
                <td class="parameter"><font>Battery voltage:</font></td>
                <td class="choice_arrow" id="line_0_c"><font>&#9658;</font></td>
                <td class="value"><font id="voltage_0">3.56 V</font></td>
            </tr>
            <tr>
                <td class="choice_arrow" id="line_1_u"><font>&#9658;</font></td>
                <td class="parameter"><font>Relay 1 threshold:</font></td>
                <td class="choice_arrow" id="line_1_c"><font>&#9658;</font></td>
                <td class="value"><font id="voltage_1">3.56 V</font></td>
            </tr>
            <tr>
                <td class="choice_arrow" id="line_2_u"><font>&#9658;</font></td>
                <td class="parameter"><font>Relay 2 threshold:</font></td>
                <td class="choice_arrow" id="line_2_c"><font>&#9658;</font></td>
                <td class="value"><font id="voltage_2">3.56 V</font></td>
            </tr>
            <tr>
                <td class="choice_arrow" id="line_3_u"><font>&#9658;</font></td>
                <td class="parameter"><font>Relay 3 threshold:</font></td>
                <td class="choice_arrow" id="line_3_c"><font>&#9658;</font></td>
                <td class="value"><font id="voltage_3">3.56 V</font></td>
            </tr>
            <tr>
                <td class="choice_arrow" id="line_4_u"><font>&#9658;</font></td>
                <td class="parameter"><font>Hysteresis:</font></td>
                <td class="choice_arrow" id="line_4_c"><font>&#9658;</font></td>
                <td class="value"><font id="voltage_4">3.56 V</font></td>
            </tr>
        </table>
        <div><button style="background-color: #a881af;" onmousedown="mouseDown_Up()" onmouseup="mouseUp_Up()">Up</button></div>
        <div><button style="background-color: #a881af;" onmousedown="mouseDown_Down()" onmouseup="mouseUp_Down()">Down</button></div>
        <div><button style="background-color: #4CAF50;" onmouseup="mouseUp_Select()">Select</button></div>
        <div><button style="background-color: #a7523f;" onmouseup="mouseUp_Reset()">Reset</button></div>
    </center>
    <script>
        var cur_prev_lines = [[1, 0],[1, 0]];
        var voltage_is_changing = false;
        var lines = [["line_0_u", "line_0_c"], ["line_1_u", "line_1_c"],
                     ["line_2_u", "line_2_c"], ["line_3_u", "line_3_c"], ["line_4_u", "line_4_c"]];
        var voltages_ids = ["voltage_0", "voltage_1", "voltage_2", "voltage_3", "voltage_4"];
        var voltages_mv = [200, 3140, 3150, 10, 20]; // save voltages in millivolts
        
        var timer_up_btn;
        var timer_down_btn;
        var timer_up_btn_worked = false;
        var timer_down_btn_worked = false;

        var old_voltage_mv = -1;    // -1 means that old voltage is unset
        var current_changing_voltage_mv;    // This is the voltage that saves the changes of certain voltage_№
                                            // while the button is working with some voltage-cell

        // Function for starting the page
        window.onload = function() {
            startWindow();
        };
        function startWindow() {
            change_choice();
            // 1 - GET voltages from the server
            // 2 - set voltages in the array
            update_info_from_server();

            refresh_voltages_on_screen();
        }

        // Move the arrow frome old place to new
        function change_choice() {
            document.getElementById(lines[cur_prev_lines[1][0]][cur_prev_lines[1][1]]).style.opacity = 0;
            document.getElementById(lines[cur_prev_lines[0][0]][cur_prev_lines[0][1]]).style.opacity = 1;
        }

        // This function gets raw voltages from the array
        // converts them and print to the screen
        function refresh_voltages_on_screen() {
            var voltage_f = 0;
            var str_voltage;
            for (var i = 0; i <= 4; i++) {
                // continue if we are changing some voltage rigth now using btns on the client
                if (cur_prev_lines[0][0] == i && voltage_is_changing)
                    continue;

                voltage_f = (voltages_mv[i] - voltages_mv[i] % 10) / 1000;
                str_voltage = voltage_f  + " V";
                // console.log(4-String(voltage_f).length);
                document.getElementById(voltages_ids[i]).innerHTML = str_voltage;
            }
        }

        // Function on mousedown for UP_BUTTON
        function mouseDown_Up() {
            if (voltage_is_changing) {
                timer_up_btn = setInterval(timer_handler_up_btn, 500);
            }
        }

        // Timer handler for UP_BUTTON
        function timer_handler_up_btn() {
            timer_up_btn_worked = true;
            var cur_line = cur_prev_lines[0][0];

            // Remember old voltage
            if (old_voltage_mv == -1) {
                old_voltage_mv = voltages_mv[cur_line];
                current_changing_voltage_mv = voltages_mv[cur_line];
            }
            current_changing_voltage_mv += 100;
            
            // border check
            if (current_changing_voltage_mv > 3150)
                current_changing_voltage_mv -= 100;

            var voltage_f = (current_changing_voltage_mv - current_changing_voltage_mv % 10) / 1000;
            str_voltage = voltage_f  + " V";
            document.getElementById(voltages_ids[cur_line]).innerHTML = str_voltage;
            clearInterval(timer_up_btn);
            // console.log("timer worked!!!");
        }

        // Function on mouseup for UP_BUTTON
        function mouseUp_Up() {
            if (!voltage_is_changing) { // Here we switch lines
                cur_prev_lines[1][0] = cur_prev_lines[0][0];
                cur_prev_lines[1][1] = cur_prev_lines[0][1];

                cur_prev_lines[0][0] -= 1;
                if (cur_prev_lines[0][0] == 0)
                    cur_prev_lines[0][0] = 4;
                cur_prev_lines[0][1] = 0;
                // console.log(cur_prev_lines[0][0]);
                change_choice();
            } else { // Here we adjust the voltage
                if (!timer_up_btn_worked) { // If timer hasn't worked
                    clearInterval(timer_up_btn);
                    var cur_line = cur_prev_lines[0][0];

                    // Remember old voltage
                    if (old_voltage_mv == -1) {
                        old_voltage_mv = voltages_mv[cur_line];
                        current_changing_voltage_mv = voltages_mv[cur_line];
                    }
                    current_changing_voltage_mv += 10;

                    // border check
                    if (current_changing_voltage_mv > 3150)
                        current_changing_voltage_mv -= 10;

                    var voltage_f = (current_changing_voltage_mv - current_changing_voltage_mv % 10) / 1000;
                    str_voltage = voltage_f  + " V";
                    document.getElementById(voltages_ids[cur_line]).innerHTML = str_voltage;
                } else { // If timer has worked
                    timer_up_btn_worked = false;
                }
            }
        }

        // Function on mousedown for DOWN_BUTTON
        function mouseDown_Down() {
            if (voltage_is_changing) {
                timer_down_btn = setInterval(timer_handler_down_btn, 500);
            }
        }

        // Timer handler for DOWN_BUTTON
        function timer_handler_down_btn() {
            timer_down_btn_worked = true;
            var cur_line = cur_prev_lines[0][0];

            // Remember old voltage
            if (old_voltage_mv == -1) {
                old_voltage_mv = voltages_mv[cur_line];
                current_changing_voltage_mv = voltages_mv[cur_line];
            }
            current_changing_voltage_mv -= 100;

            // border check
            if (current_changing_voltage_mv < 0)
                current_changing_voltage_mv += 100;

            var voltage_f = (current_changing_voltage_mv - current_changing_voltage_mv % 10) / 1000;
            str_voltage = voltage_f  + " V";
            document.getElementById(voltages_ids[cur_line]).innerHTML = str_voltage;
            clearInterval(timer_down_btn);
            // console.log("timer worked!!!");
        }

        // Function on mouseup for DOWN_BUTTON
        function mouseUp_Down() {
            if (!voltage_is_changing) { // Here we switch lines
                cur_prev_lines[1][0] = cur_prev_lines[0][0];
                cur_prev_lines[1][1] = cur_prev_lines[0][1];
                
                cur_prev_lines[0][0] += 1;
                if (cur_prev_lines[0][0] == 5)
                    cur_prev_lines[0][0] = 1;
                cur_prev_lines[0][1] = 0;
                // console.log(cur_prev_lines[0][0]);
                change_choice();
            } else { // Here we adjust the voltage
                if (!timer_down_btn_worked) { // If timer hasn't worked
                    clearInterval(timer_down_btn);
                    var cur_line = cur_prev_lines[0][0];

                    // Remember old voltage
                    if (old_voltage_mv == -1) {
                        old_voltage_mv = voltages_mv[cur_line];
                        current_changing_voltage_mv = voltages_mv[cur_line];
                    }
                    current_changing_voltage_mv -= 10;

                    // border check
                    if (current_changing_voltage_mv < 0)
                        current_changing_voltage_mv += 10;

                    var voltage_f = (current_changing_voltage_mv - current_changing_voltage_mv % 10) / 1000;
                    str_voltage = voltage_f  + " V";
                    document.getElementById(voltages_ids[cur_line]).innerHTML = str_voltage;
                } else { // If timer has worked
                    timer_down_btn_worked = false;
                }
            }
        }

        function mouseUp_Select() {
            if (!voltage_is_changing) {
                voltage_is_changing = true;
                cur_prev_lines[1][0] = cur_prev_lines[0][0];
                cur_prev_lines[1][1] = cur_prev_lines[0][1];
                cur_prev_lines[0][1] = 1;
                change_choice();            
            } else {
                voltage_is_changing = false;

                // Update new voltage in the array
                var cur_line = cur_prev_lines[0][0];
                voltages_mv[cur_line] = current_changing_voltage_mv;
                
                // Change the old_voltage int order to it will be able
                // to get new old voltage next time)))
                old_voltage_mv = -1;

                send_info_to_server()

                cur_prev_lines[1][0] = cur_prev_lines[0][0];
                cur_prev_lines[1][1] = cur_prev_lines[0][1];
                cur_prev_lines[0][1] = 0;
                refresh_voltages_on_screen();
                change_choice();            
            }
        }

        function mouseUp_Reset() {
            if (voltage_is_changing) {
                voltage_is_changing = false;

                // Return old voltage
                var cur_line = cur_prev_lines[0][0];
                voltages_mv[cur_line] = old_voltage_mv;
                old_voltage_mv = -1;

                cur_prev_lines[1][0] = cur_prev_lines[0][0];
                cur_prev_lines[1][1] = cur_prev_lines[0][1];
                cur_prev_lines[0][1] = 0;
                refresh_voltages_on_screen();
                change_choice();
            }
        }

        //
        //        <--------------WORKING WITH THE HTTP-------------->
        //
        
        // Gets info from the server and puts it to the array of voltages
        function update_info_from_server() {
            
            const getVoltagesURL = window.location.href + "voltages";
            const xhr = new XMLHttpRequest();
            xhr.open('GET', getVoltagesURL);
            xhr.onload = () => {
                var array_v = xhr.responseText.split(";");
                for (var i = 0; i < array_v.length; i++) {
                    voltages_mv[i] = Number(array_v[i]);
                }
            }
            xhr.send();

            refresh_voltages_on_screen();
        }

        setInterval(update_info_from_server, 1000);  // gets the info from the server once in 2 seconds
        
        // send new info to the server
        function send_info_to_server() {
            // console.log("SEND INFO TO SERVER!");

            const params = new URLSearchParams();
            params.append('Bat_V', String(voltages_mv[0]));
            params.append('R_1_V', String(voltages_mv[1]));
            params.append('R_2_V', String(voltages_mv[2]));
            params.append('R_3_V', String(voltages_mv[3]));
            params.append('His_V', String(voltages_mv[4]));
            const paramsString = params.toString();
            const postVoltagesURL = `${window.location.href}voltages?${paramsString}`;

            const xhr = new XMLHttpRequest();
            xhr.open('POST', postVoltagesURL);
            xhr.send();
        }

    </script>
</body>
</html>
)=====";