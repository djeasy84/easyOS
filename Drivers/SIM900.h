/*
 * MIT License
 *
 * This file is part of easyOS.
 * You can find the software at https://github.com/djeasy84/easyOS
 *
 * Copyright (c) 2018 - Lorenzo Baglioni (djeasy84) <lbaglioni84@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef GSM_H
#define GSM_H

#define GSM_CMD_MAX_STRING 30
#define GSM_SMS_MAX_STRING 60
#define GSM_PHONE_MAX_STRING 15

enum enum_gsm { error, powerup, escape, reset, noecho, config, running, sendsms };
enum enum_gsm_cmd { command, response, state_1, state_2, state_3, state_4 };
enum enum_gsm_cmd_result { ERROR, OK, WAIT, RING };

class GSMEngine
{
	public:
		bool setup(uint8_t pwr, uint8_t port);

		bool sendSMS(char *smsPhoneNumber, char *smsMessage);
		bool incomingCall(void);

		bool update();

	private:
		uint8_t pwr_pin;
		uint8_t portNUM;

		bool gsm_ready;
		bool sms_requested;
		bool incoming_call;

		enum_gsm gsm_state;
		enum_gsm_cmd gsm_cmd_state;
		unsigned long cmd_exec_time;
		unsigned long cmd_delay_timeout;
		unsigned long cmd_response_timeout;

		char message_text[GSM_SMS_MAX_STRING];
		char message_phone[GSM_PHONE_MAX_STRING];
		char cmd_response_data[GSM_CMD_MAX_STRING];

		unsigned long sms_last_time;

		enum_gsm_cmd_result cmdPowerUp(void);
		enum_gsm_cmd_result cmdEscape(void);
		enum_gsm_cmd_result cmdReset(void);
		enum_gsm_cmd_result cmdNoEcho(void);
		enum_gsm_cmd_result cmdSMSConfig(void);
		enum_gsm_cmd_result cmdNetwork(void);
		enum_gsm_cmd_result cmdSendSMS(char *phone, char *message);

		enum_gsm_cmd_result sendCommand(char *command);
		enum_gsm_cmd_result readCommand(char *response, char *response_data = 0x00);
};

GSMEngine GSM;

/****************************************************************************************/

bool GSMEngine::setup(uint8_t pwr, uint8_t port)
{
	portNUM = 0;
    if (port != 1 && port != 2 && port != 3)
        return false;

	pwr_pin = pwr;
	portNUM = port;

	gsm_ready = false;
	sms_requested = false;
	incoming_call = false;

	gsm_state = error;
	gsm_cmd_state = command;
	cmd_exec_time = 0;
	cmd_delay_timeout = 0;
	cmd_response_timeout = 0;

	memset(message_text, 0, GSM_SMS_MAX_STRING);
	memset(message_phone, 0, GSM_PHONE_MAX_STRING);
	memset(cmd_response_data, 0, GSM_CMD_MAX_STRING);

	sms_last_time = 0;

	DP.write(pwr_pin, false);

	switch(portNUM)
    {
        #if defined (SERIAL_HW_1_M)
        case 1:
            SHW1.setup();
			SHW1.flush();
        break;
        #endif
        #if defined (SERIAL_HW_2_M)
        case 2:
            SHW2.setup();
			SHW2.flush();
        break;
        #endif
        #if defined (SERIAL_HW_3_M)
        case 3:
            SHW3.setup();
			SHW3.flush();
        break;
        #endif
    }
}

bool GSMEngine::sendSMS(char *smsPhoneNumber, char *smsMessage)
{
	if (sms_requested || !gsm_ready)
		return false;
	strcpy(message_text, smsMessage);
	strcpy(message_phone, smsPhoneNumber);
	sms_requested = true;
	return true;
}

bool GSMEngine::update()
{
	if (portNUM == 0)
        return false;

	if (!sms_requested)
	{
		sms_last_time = 0;
	}

	if (ST.time_diff(ST.millisec(), cmd_exec_time) < cmd_delay_timeout)
	{
		return gsm_ready;
	}

	switch(gsm_state)
	{
		case error:
		{
			gsm_ready = false;
			gsm_state = powerup;
			gsm_cmd_state = command;
			cmd_exec_time = ST.millisec();
			cmd_delay_timeout = 5000;
			break;
		}
		case powerup:
		{
			enum_gsm_cmd_result res = cmdPowerUp();
			if (res == OK)
			{
				gsm_state = escape;
				gsm_cmd_state = command;
				cmd_exec_time = ST.millisec();
				cmd_delay_timeout = 5000;
			}
			else if (res == ERROR)
			{
				gsm_state = error;
				gsm_cmd_state = command;
				cmd_exec_time = 0;
				cmd_delay_timeout = 0;
			}
			break;
		}
		case escape:
		{
			enum_gsm_cmd_result res = cmdEscape();
			if (res == OK)
			{
				gsm_state = reset;
				gsm_cmd_state = command;
				cmd_exec_time = ST.millisec();
				cmd_delay_timeout = 5000;
			}
			else if (res == ERROR)
			{
				gsm_state = error;
				gsm_cmd_state = command;
				cmd_exec_time = 0;
				cmd_delay_timeout = 0;
			}
			break;
		}
		case reset:
		{
			enum_gsm_cmd_result res = cmdReset();
			if (res == OK)
			{
				gsm_state = noecho;
				gsm_cmd_state = command;
				cmd_exec_time = ST.millisec();
				cmd_delay_timeout = 5000;
			}
			else if (res == ERROR)
			{
				gsm_state = error;
				gsm_cmd_state = command;
				cmd_exec_time = 0;
				cmd_delay_timeout = 0;
			}
			break;
		}
		case noecho:
		{
			enum_gsm_cmd_result res = cmdNoEcho();
			if (res == OK)
			{
				gsm_state = config;
				gsm_cmd_state = command;
				cmd_exec_time = ST.millisec();
				cmd_delay_timeout = 5000;
			}
			else if (res == ERROR)
			{
				gsm_state = error;
				gsm_cmd_state = command;
				cmd_exec_time = 0;
				cmd_delay_timeout = 0;
			}
			break;
		}
		case config:
		{
			enum_gsm_cmd_result res = cmdSMSConfig();
			if (res == OK)
			{
				gsm_state = running;
				gsm_cmd_state = command;
				cmd_exec_time = ST.millisec();
				cmd_delay_timeout = 5000;
			}
			else if (res == ERROR)
			{
				gsm_state = error;
				gsm_cmd_state = command;
				cmd_exec_time = 0;
				cmd_delay_timeout = 0;
			}
			break;
		}
		case running:
		{
			enum_gsm_cmd_result res = cmdNetwork();
			if (res == OK)
			{
				if (sms_requested && ((ST.time_diff(ST.millisec(), sms_last_time) > 60000*5) || sms_last_time == 0))
				{
					gsm_state = sendsms;
					gsm_cmd_state = command;
					cmd_exec_time = ST.millisec();
					cmd_delay_timeout = 1000;
				}
				else
				{
					gsm_state = running;
					gsm_cmd_state = command;
					cmd_exec_time = ST.millisec();
					cmd_delay_timeout = 1000;
				}
			}
			else if (res == ERROR)
			{
				gsm_state = error;
				gsm_cmd_state = command;
				cmd_exec_time = 0;
				cmd_delay_timeout = 0;
			}
			break;
		}
		case sendsms:
		{
			enum_gsm_cmd_result res = cmdSendSMS(message_phone, message_text);
			if (res == OK)
			{
				gsm_state = running;
				gsm_cmd_state = command;
				cmd_exec_time = ST.millisec();
				cmd_delay_timeout = 1000;

				sms_requested = false;
				strcpy(message_text, "");
				sms_last_time = ST.millisec();
			}
			else if (res == ERROR)
			{
				gsm_state = error;
				gsm_cmd_state = command;
				cmd_exec_time = 0;
				cmd_delay_timeout = 0;
			}
			break;
		}
	}
	return gsm_ready;
}

bool GSMEngine::incomingCall(void)
{
	return incoming_call;
}

enum_gsm_cmd_result GSMEngine::cmdPowerUp(void)
{
	if (gsm_cmd_state == command)
	{
		DP.write(pwr_pin, false);
		gsm_cmd_state = state_1;
		cmd_exec_time = ST.millisec();
		cmd_delay_timeout = 1000;
		return WAIT;
	}
	else if (gsm_cmd_state == state_1)
	{
		DP.write(pwr_pin, true);
		gsm_cmd_state = state_2;
		cmd_exec_time = ST.millisec();
		cmd_delay_timeout = 1000;
		return WAIT;
	}
	else if (gsm_cmd_state == state_2)
	{
		DP.write(pwr_pin, false);
		gsm_cmd_state = response;
		cmd_exec_time = ST.millisec();
		cmd_delay_timeout = 1000;
		return WAIT;
	}
	else if (gsm_cmd_state == response)
	{
		return OK;
	}
	return ERROR;
}

enum_gsm_cmd_result GSMEngine::cmdEscape(void)
{
	if (gsm_cmd_state == command)
	{
		char submit_cmd[2] = { 0x1B, 0x00 };
		if (sendCommand(submit_cmd) == OK)
		{
			gsm_cmd_state = response;
			return WAIT;
		}
	}
	else if (gsm_cmd_state == response)
	{
		return OK;
	}
	return ERROR;
}

enum_gsm_cmd_result GSMEngine::cmdReset(void)
{
	if (gsm_cmd_state == command)
	{
		if (sendCommand("AT&F") == OK)
		{
			gsm_cmd_state = response;
			return WAIT;
		}
	}
	else if (gsm_cmd_state == response)
	{
		return readCommand("OK");
	}
	return ERROR;
}

enum_gsm_cmd_result GSMEngine::cmdNoEcho(void)
{
	if (gsm_cmd_state == command)
	{
		if (sendCommand("ATE0") == OK)
		{
			gsm_cmd_state = response;
			return WAIT;
		}
	}
	else if (gsm_cmd_state == response)
	{
		return readCommand("OK");
	}
	return ERROR;
}

enum_gsm_cmd_result GSMEngine::cmdSMSConfig(void)
{
	if (gsm_cmd_state == command)
	{
		if (sendCommand("AT+CMGF=1") == OK)
		{
			gsm_cmd_state = response;
			return WAIT;
		}
	}
	else if (gsm_cmd_state == response)
	{
		return readCommand("OK");
	}
	return ERROR;
}

enum_gsm_cmd_result GSMEngine::cmdNetwork(void)
{
	if (gsm_cmd_state == command)
	{
		if (sendCommand("AT+CREG?") == OK)
		{
			gsm_cmd_state = response;
			return WAIT;
		}
	}
	else if (gsm_cmd_state == response)
	{
		char response_data[GSM_CMD_MAX_STRING];
		memset(response_data, 0, GSM_CMD_MAX_STRING);
		enum_gsm_cmd_result res = readCommand("OK", response_data);
		if (res == OK)
		{
			if (response_data[9] == '1' || response_data[9] == '5')
			{
				gsm_ready = true;
				return OK;
			}
			else
			{
				gsm_ready = false;
				return WAIT;
			}
		}
		else if (res == WAIT)
		{
			return WAIT;
		}
	}
	return ERROR;
}

enum_gsm_cmd_result GSMEngine::cmdSendSMS(char *phone, char *message)
{
	if (phone == 0x00 || message == 0x00)
	{
		return ERROR;
	}
	if (gsm_cmd_state == command)
	{
		char command[GSM_CMD_MAX_STRING];
		memset(command, 0, GSM_CMD_MAX_STRING);
		strcat(command, "AT+CMGS=\"+39");
		strcat(command, phone);
		strcat(command, "\"");
		if (sendCommand(command) == OK)
		{
			gsm_cmd_state = state_1;
			return WAIT;
		}
	}
	else if (gsm_cmd_state == state_1)
	{
		enum_gsm_cmd_result res = readCommand(">");
		if (res == OK)
		{
			gsm_cmd_state = state_2;
			return WAIT;
		}
		else if (res == WAIT)
		{
			return WAIT;
		}
	}
	else if (gsm_cmd_state == state_2)
	{
		if (sendCommand(message) == OK)
		{
			gsm_cmd_state = state_3;
			return WAIT;
		}
	}
	else if (gsm_cmd_state == state_3)
	{
		enum_gsm_cmd_result res = readCommand(">");
		if (res == OK)
		{
			gsm_cmd_state = state_4;
			return WAIT;
		}
		else if (res == WAIT)
		{
			return WAIT;
		}
	}
	else if (gsm_cmd_state == state_4)
	{
		char submit_cmd[2] = { 0x1A, 0x00 };
		if (sendCommand(submit_cmd) == OK)
		{
			gsm_cmd_state = response;
			return WAIT;
		}
	}
	else if (gsm_cmd_state == response)
	{
		return readCommand("OK");
	}
	return ERROR;
}

enum_gsm_cmd_result GSMEngine::sendCommand(char *command)
{
	switch(portNUM)
    {
        #if defined (SERIAL_HW_1_M)
        case 1:
		{
            bool res = SHW1.write((unsigned char *)command, strlen(command));
			if (res == false)
			{
				return ERROR;
			}
			res = SHW1.write((unsigned char *)"\r", 1);
			if (res == false)
			{
				return ERROR;
			}
        	break;
		}
        #endif
        #if defined (SERIAL_HW_2_M)
        case 2:
		{
            bool res = SHW2.write((unsigned char *)command, strlen(command));
			if (res == false)
			{
				return ERROR;
			}
			res = SHW2.write((unsigned char *)"\r", 1);
			if (res == false)
			{
				return ERROR;
			}
        	break;
		}
        #endif
        #if defined (SERIAL_HW_3_M)
        case 3:
		{
            bool res = SHW3.write((unsigned char *)command, strlen(command));
			if (res == false)
			{
				return ERROR;
			}
			res = SHW3.write((unsigned char *)"\r", 1);
			if (res == false)
			{
				return ERROR;
			}
        	break;
		}
        #endif
    }
	cmd_response_timeout = ST.millisec();
	memset(cmd_response_data, 0, GSM_CMD_MAX_STRING);
	return OK;
}

enum_gsm_cmd_result GSMEngine::readCommand(char *response, char *response_data)
{
	if (ST.time_diff(ST.millisec(), cmd_response_timeout) > 60000)
	{
		return ERROR;
	}
	char buffer[GSM_CMD_MAX_STRING];
	memset(buffer, 0, GSM_CMD_MAX_STRING);
	uint8_t count = 0;
	switch(portNUM)
    {
        #if defined (SERIAL_HW_1_M)
        case 1:
            SHW1.readLineCR(buffer, &count, GSM_CMD_MAX_STRING);
        break;
        #endif
        #if defined (SERIAL_HW_2_M)
        case 2:
            SHW2.readLineCR(buffer, &count, GSM_CMD_MAX_STRING);
        break;
        #endif
        #if defined (SERIAL_HW_3_M)
        case 3:
			SHW3.readLineCR(buffer, &count, GSM_CMD_MAX_STRING);
        break;
        #endif
    }
	if (count <= 0)
	{
		return WAIT;
	}
	if (strstr(buffer, "POWER DOWN") || strstr(buffer, "ERROR"))
	{
		return ERROR;
	}
	if (strstr(buffer, "RING"))
	{
		incoming_call = true;
		return WAIT;
	}
	if (strstr(buffer, "NO CARRIER"))
	{
		incoming_call = false;
		return WAIT;
	}
	if (strstr(buffer, response))
	{
		if (strlen(cmd_response_data) != 0 && response_data != 0x00)
		{
			memcpy(response_data, cmd_response_data, GSM_CMD_MAX_STRING);
		}
		return OK;
	}
	if (strstr(buffer, "\n+") && strlen(cmd_response_data) == 0)
	{
		strncat(cmd_response_data, buffer+1, GSM_CMD_MAX_STRING-1);
	}
	return WAIT;
}

#endif
