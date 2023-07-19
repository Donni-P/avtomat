#pragma once
template<class CE>
class ProtocoleAvt{
public:
    enum state{
        IGNORE,
        AMOUNT,
        STEP1,
        STEP2
    };
    bool isEndCom(char byte){
        if(byte == '#'){
            avt_state = IGNORE;
            switch(com){
                case 1:
                    CE::execute_move(amount,buffer);
                    break;
                case 2:
                    CE::execute_waitTicks(amount,buffer);
                    break;
                case 3:
                    CE::execute_scan(amount,buffer);
                    break;
                case 4:
                    CE::execute_waitTime(amount,buffer);
                    break;
            }
            return true;
        }else return false;
    }
    void readByte(char byte){
        switch(avt_state){

            case IGNORE:
            //B b R 0 1 2 3 4 5 6 7 r t s u h - обработчик

                com = 0;
                cnt_byte = 0;
                /*if(byte == '$'){
                    avt_state = AMOUNT;     *БЫЛО*
                }*/
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
                    if((com < 1)||(com > 4)){
                        avt_state = IGNORE;
                        return;
                    }
                } 

                amount = amount * 16 + hexToInt(byte);
                if(--cnt_byte == 0){
                    if(!isEndCom(byte) && ((com == 1)||(com == 3))){
                        cnt_byte = 0;
                        initAmountByte();
                        avt_state = STEP1;
                    }
                    
                }
                break;
            case STEP1:
                buffer[cnt_byte] = hexToInt(byte);
                if(cnt_byte != amount/8)
                    avt_state = STEP2;
                break;
            case STEP2:
                buffer[cnt_byte] += (hexToInt(byte) * 16);
                cnt_byte++;
                if(cnt_byte != amount/8)
                    avt_state = STEP1;
                break;

        }
    }
private:
    void initAmountByte(){
        amount_byte = amount / 8;
        if(amount % 8 != 0)
            amount_byte++;
    }
    int hexToInt(unsigned char x){
        if (x>='0' && x<='9'){return x-'0';}
        if (x>='a' && x<='f'){return x-'a'+10;}
        if (x>='A' && x<='F'){return x-'A'+10;}
        return 0;
    }
    state avt_state = IGNORE;
    int amount = 0;
    int amount_byte = 0;
    int cnt_byte = 0;
    int com = 0;
    int buffer[512];
};