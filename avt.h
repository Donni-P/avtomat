#pragma once
template<class CE>
class ProtocoleAvt{
public:
    enum state{
        IGNORE,
        AMOUNT,
        HALF1BYTE,
        HALF2BYTE,
        ERROR
    };
    bool isEndCom(char byte){
        if(byte == '#'){
            avt_state = IGNORE;
            if(amount == 0){
                CE::error_nullAmount();
            }else{
                switch(com){
                    case 1:
                        CE::execute_move(amount,buffer);
                        break;
                    case 2:
                        CE::execute_waitTicks(amount);
                        break;
                    case 3:
                        CE::execute_scan(amount,buffer);
                        break;
                    case 4:
                        CE::execute_waitTime(amount);
                        break;
                    case 5:
                        CE::execute_setSpeed(amount);
                }
            }
            return true;
        }else return false;
    }
    bool toNextHalfByte(char byte, state nextHalfState){
        if(!isEndCom(byte) && cnt_byte == amount_halfByte){
            avt_state = ERROR;
            CE::error_overflow();
            return false;
        }else{
            if(cnt_byte < amount_halfByte){
                avt_state = nextHalfState;
            }
            return true;
        }
    }
    void readByte(char byte){
        if(
            ((avt_state == AMOUNT) && ((byte == '#') || (byte == '$')) && (cnt_byte != 0)) ||
            ((avt_state == HALF1BYTE || avt_state == HALF2BYTE) && (cnt_byte < amount_halfByte) && ((byte == '#') || (byte == '$')))
        ){
            if(cnt_byte == com)
                CE::error_unknownCommand();
            else CE::error_undercount();
            avt_state = ERROR;
        }
        int char_toInt = 0;
        switch(avt_state){
            case IGNORE:
                clearFields();
                switch(byte){
                    case '$':
                        avt_state = AMOUNT;
                        break;
                    case 'B':
                        CE::execute_B();
                        break;
                    case 'b':
                        CE::execute_b();
                        break;
                    case 'R':
                        CE::execute_R();
                        break;
                    case '0':
                        CE::execute_0();
                        break;
                    case '1':
                        CE::execute_1();
                        break;
                    case '2':
                        CE::execute_2();
                        break;
                    case '3':
                        CE::execute_3();
                        break;
                    case '4':
                        CE::execute_4();
                        break;
                    case '5':
                        CE::execute_5();
                        break;
                    case '6':
                        CE::execute_6();
                        break;
                    case '7':
                        CE::execute_7();
                        break;
                    case 'r':
                        CE::execute_r();
                        break;
                    case 't':
                        CE::execute_t();
                        break;
                    case 's':
                        CE::execute_s();
                        break;
                    case 'u':
                        CE::execute_u();
                        break;
                    case 'h':
                        CE::execute_h();
                        break;
                    
                }
                break;

            case AMOUNT:
                if(com == 0){
                    com = byte - '0';
                    cnt_byte = com;
                    if((com < 1) || (com > 5)){
                        if(byte != '$')
                            avt_state = ERROR;
                        else avt_state = IGNORE;
                        CE::error_unknownCommand();
                    }
                }else if(cnt_byte-- != 0){
                    char_toInt = hexToInt(byte);
                    amount = amount * 16 + char_toInt;
                    if(((com == 1) || (com == 3)) && (cnt_byte == 0)){
                        initAmountByte();
                        avt_state = HALF1BYTE;
                    }
                }else {
                    if(!isEndCom(byte)){
                        avt_state = ERROR;
                        CE::error_overflow();
                    }
                }
                break;

            case HALF1BYTE:
                if(toNextHalfByte(byte,HALF2BYTE)){
                    char_toInt = hexToInt(byte);
                    buffer[cnt_byte/2] = char_toInt;
                    cnt_byte++;
                }
                break;

            case HALF2BYTE:
                if(toNextHalfByte(byte,HALF1BYTE)){
                    char_toInt = hexToInt(byte);
                    buffer[cnt_byte/2] += (char_toInt * 16);
                    cnt_byte++;
                }
                break;
            case ERROR:
                switch(byte){
                    case '$':
                        clearFields();
                        avt_state = AMOUNT;
                        break;
                    case '#':
                        avt_state = IGNORE;
                        break;
                }
                break;
        }
        if((char_toInt == -1) && (avt_state != IGNORE) && (avt_state != ERROR)){
            avt_state = ERROR;
            CE::error_wrongChar();
        }
    }
private:
    void initAmountByte(){
        amount_halfByte = amount / 4;
        if(amount % 4 != 0)
            amount_halfByte++;
    }
    void clearFields(){
        amount = 0;
        com = 0;
        cnt_byte = 0;
        for(int i = 0; buffer[i] != 0; i++)
            buffer[i] = 0;
    }
    int hexToInt(unsigned char x){
        if (x>='0' && x<='9'){return x-'0';}
        if (x>='a' && x<='f'){return x-'a'+10;}
        if (x>='A' && x<='F'){return x-'A'+10;}
        return -1;
    }
    state avt_state = IGNORE;
    int amount = 0;
    int amount_halfByte = 0;
    int cnt_byte = 0;
    int com = 0;
    int buffer[512] = {0};
};