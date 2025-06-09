#include<iostream>
#include<string>
#include<vector>

// 前向宣告
class Specific;
class Regular;
class Booking;
class Passenger;

class Booking//訂票
{
private:
    std::string seatNumber;//座位號碼
    Passenger* passenger;//此訂單的乘客
    Specific* specificTrain;//此訂單代表的特定車次
    static std::vector<Booking*> allBookings; // 儲存所有訂票記錄
public:
    //Constructor
    Booking(const std::string& seatNumber):seatNumber(seatNumber),passenger(nullptr),specificTrain(nullptr){
    };
    //Destructor
    ~Booking(){};

    //Setters
    void setSeatNumber(const std::string& s) { seatNumber = s; }
    void setPassenger(Passenger* p){
        passenger=p;
        allBookings.push_back(this); // 在建構時將訂票加入記錄中
    }
    //Getters
    std::string getSeatNumber() const { return seatNumber; }
    Passenger* getPassenger() const { return passenger; }
    Specific* getSpecificTrain() const { return specificTrain; }

    //設定此訂單代表的特定車次
    void setSpecificTrain(Specific* s);
    // 顯示訂票資訊 
    void displayBookingInfo() const;
    //用身分證查詢訂票資訊
    static Booking* findBooking(const std::string& number);
    //用身分證查詢該乘客的所有訂票資訊
    static void findAllBookingsByNumber(const std::string& number);
};

class Passenger//乘客
{
private:
    std::string name;//乘客姓名
    std::string number;//身分證字號
    std::vector<Booking*> bookings;//乘客訂的票
public:
    //Constructor
    Passenger(const std::string& n,const std::string& num):name(n),number(num){};
    //Destructor
    ~Passenger(){};
    
    //Setters
    void setName(const std::string& n) { name = n; }
    void setNumber(const std::string& num) { number = num; }
    
    //Getters
    std::string getName() const { return name; }
    std::string getNumber() const { return number; }

    //乘客訂票
    void books(Booking* b){
        bookings.push_back(b);
        b->setPassenger(this);
    };
};
class Regular//定期車次
{
private:
    std::string number;//車次號碼
    std::string time;//車次時間
    //車次號碼只允許3碼
    bool isValidTrainNumber(const std::string& number) const {
        return number.length() == 3;
    }
    std::vector<Specific*>specificTrains;//定期車次所擁有的特定車次
public:
    //Constructor
    Regular(const std::string& n,const std::string& t) :number(n),time(t){}
    
    ~Regular() {}

    // Setters
    void setTrainNumber(const std::string& n) {
        if (!isValidTrainNumber(n)) {
            throw std::invalid_argument("車次號碼必須為3碼");
        }
        number = n;
    }
    // Getters
    std::string getTrainNumber() const { return number; }
    std::string getTime() const { return time; }
    //增加此定期車次的特定車次(同時設該特定車次之所屬定期車次)
    void addSpecific(Specific* spec);
    
    //此定期車次有哪些特定車次
    void displaySpecific() const;
};

class Specific //特定車次
{
private:
    std::string date; // 班次日期
    Regular *regularTrain;//特定車次所屬的定期車次
    std::vector<Booking*> bookings;//此特定車次所擁有的訂單(座位號碼)
    static std::vector<Specific*> allSpecifics; // 存儲所有 Specific 物件的指標
public:
    //Construtor
    Specific(const std::string& d) : date(d), regularTrain(nullptr) {
        allSpecifics.push_back(this); // 將當前物件添加到靜態集合
    }
    //Destructor
    ~Specific(){}
    //Setter
    void setDate(const std::string& d){
        date=d;
    }
    void setRegular(Regular* r){
        regularTrain=r;
    }
    // Getter
    std::string getDate() const {
        return date;
    }
    Regular* getRegular() const { return regularTrain; }
    //設定此特定車次所擁有的訂單(座位號碼)
    void setBooking(Booking* b){    
        bookings.push_back(b);
        b->setSpecificTrain(this);
    }
    //用班次日期查詢所有訂票資訊
    static void findAllBookingsByTime(const std::string& time);
};

// Regular函數實作
void Regular::addSpecific(Specific* spec){
    specificTrains.push_back(spec);
    spec->setRegular(this);
}
void Regular::displaySpecific() const{
    for(size_t i=0;i<specificTrains.size();i++){
        std::cout << "\t("<<i+1<<")";
        std::cout<<specificTrains[i]->getDate()<<std::endl;
    }
}

//Specific函數實作
void Specific::findAllBookingsByTime(const std::string& time) {
    bool found = false;
    std::cout << "\n以下是啟程日期為 " << time << " 的所有訂票資訊：" << std::endl;
    
    // 遍歷所有 Specific 物件
    for (Specific* spec : allSpecifics) {
        // 檢查日期是否匹配
        if (spec->getDate() == time) {
            // 遍歷此 Specific 物件的所有訂票
            for (size_t i = 0; i < spec->bookings.size(); i++) {
                spec->bookings[i]->displayBookingInfo();
                found = true;
            }
        }
    }
    
    if (!found) {
        std::cout << "找不到該時間的班次訂票記錄。" << std::endl;
    }
}

// Booking函數實作
void Booking::setSpecificTrain(Specific* s){
    specificTrain = s;
}
void Booking::displayBookingInfo() const {
    if (passenger) {
        std::cout << "乘客：" << passenger->getName();
    }
    if (specificTrain) {
        if (specificTrain->getRegular()) {
            std::cout << "\t車次：" << specificTrain->getRegular()->getTrainNumber();
            std::cout << "\t車次時間：" << specificTrain->getRegular()->getTime();
        }
        std::cout << "\t座位：" << seatNumber;
        std::cout << "\t啟程日期：" << specificTrain->getDate();
    }

    std::cout << std::endl;
}

//用身分證查詢該乘客的所有訂票資訊
void Booking::findAllBookingsByNumber(const std::string& number) {
    bool found = false;
    std::cout << "\n以下是身分證號碼 " << number << " 的所有訂票資訊：" << std::endl;
    for (Booking* booking : allBookings) {
        if (booking->getPassenger() && booking->getPassenger()->getNumber() == number) {
            booking->displayBookingInfo();
            found = true;
        }
    }
    if (!found) {
        std::cout << "找不到該身分證號碼的訂票記錄。" << std::endl;
    }
}

// 初始化靜態成員
std::vector<Booking*> Booking::allBookings;
std::vector<Specific*> Specific::allSpecifics;

int main(){
    try {
        //建立定期車次(車次號碼,車次時間)
        Regular r1("123","06:00");
 
  
        //建立特定車次(123車次有6班車)
        Specific s1("05/05");
        Specific s2("05/06"); 
        Specific s3("05/07"); 
        Specific s4("05/08"); 
        Specific s5("05/09"); 
        Specific s6("05/10"); 
        r1.addSpecific(&s1); 
        r1.addSpecific(&s2); 
        r1.addSpecific(&s3); 
        r1.addSpecific(&s4); 
        r1.addSpecific(&s5); 
        r1.addSpecific(&s6); 

        //建立乘客(姓名,身分證號碼)
        Passenger p1("張三","A123456789");
        Passenger p2("李四","B123456789");
        Passenger p3("王五","C123456789");
        Passenger p4("趙六","D123456789");
        Passenger p5("孫七","E123456789");
        Passenger p6("周八","F123456789");
        
        //建立訂票(座位號碼)
        Booking b1a("1A");
        Booking b1b("1B");
        Booking b1c("1C");

        Booking b2a("1A");
        Booking b2b("1B");
        Booking b2c("1C");

        Booking b3a("1A");
        Booking b3b("1B");
        Booking b3c("1C");
        
        Booking b4("1A");
        Booking b5("1A");
        Booking b6("1A");
        
        //設定特定車次可訂的座位

        s1.setBooking(&b1a);
        s1.setBooking(&b1b);
        s1.setBooking(&b1c);

        s2.setBooking(&b2a);
        s2.setBooking(&b2b);
        s2.setBooking(&b2c);

        s3.setBooking(&b3a);
        s3.setBooking(&b3b);
        s3.setBooking(&b3c);

        s4.setBooking(&b4);

        s5.setBooking(&b5);     

        s6.setBooking(&b6);

        //乘客訂票
        p1.books(&b1a);
        p1.books(&b4);
        p1.books(&b5);
        p2.books(&b1b);
        p3.books(&b1c);
        p4.books(&b6);
   
        //用身分證查詢該乘客訂票資訊
        std::string number;
        std::cout << "請輸入身分證號碼：";
        std::cin >> number;
        
        // 查詢該乘客的所有訂票
        Booking::findAllBookingsByNumber(number);

        // 查詢特定班次所有訂票客資訊
        std::string time;
        std::cout << "請輸入查詢班次日期：";
        std::cin >> time;
        Specific::findAllBookingsByTime(time);
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "錯誤：" << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "發生未預期的錯誤：" << e.what() << std::endl;
        return 1;
    }

    return 0;
}