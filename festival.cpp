#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;
const int daysInMonthTable[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const string monthNames[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

class Festival {
private:
    string name; // 节日名称
    int month; // 节日月份
    int day; // 节日日期
    int type; // 节日类型，1表示固定日期，2表示动态日期
public:
    // 构造函数
    Festival(string name, int month, int day, int type) {
        this->name = name;
        this->month = month;
        this->day = day;
        this->type = type;
    }

    // 获取节日名称
    string getName() const {
        return name;
    }

    // 获取节日月份
    int getMonth() const {
        return month;
    }

    // 获取节日日期
    int getDay() const {
        return day;
    }

    // 获取节日类型
    int getType() const {
        return type;
    }

    // 判断是否为指定日期的节日
    bool isFestival(int month, int day) const {
        if (this->month == month && this->day == day) {
            return true;
        }
        return false;
    }

    // 计算距离指定日期还有多少天
    int daysTo(int month, int day) const {
        int days = 0;
        // TODO: 计算距离指定日期还有多少天
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        int year = timeinfo->tm_year + 1900; // 年份需要加上 1900
        int month_today = timeinfo->tm_mon + 1;
        int day_today = timeinfo->tm_mday;
        int days = 0;
        if (month < month_today || (month == month_today && day < day_today)) {
            // 如果指定节日在今天之前，计算距离下一年的指定节日的天数
            tm timeinfo_festival = *timeinfo;
            timeinfo_festival.tm_year = year;
            timeinfo_festival.tm_mon = month - 1;
            timeinfo_festival.tm_mday = day;
            time_t festival = mktime(&timeinfo_festival);
            tm* timeinfo_next_year = localtime(&festival);
            timeinfo_next_year->tm_year++; // 下一年
            time_t next_year = mktime(timeinfo_next_year);
            days = (next_year - now) / (24 * 3600); // 计算天数
        }
        else {
            // 否则计算距离今年的指定节日的天数
            tm timeinfo_festival = *timeinfo;
            timeinfo_festival.tm_year = year - 1900;
            timeinfo_festival.tm_mon = month - 1;
            timeinfo_festival.tm_mday = day;
            time_t festival = mktime(&timeinfo_festival);
            days = (festival - now) / (24 * 3600); // 计算天数
        }
        return days;

    }
};

class FestivalManager {
private:
    vector<Festival> festivals; // 节日列表
public:
    bool hasFestival(string name) const {
        for (const auto& f : festivals) {
            if (f.getName() == name) {
                return true;
            }
        }
        return false;
    }
    // 从文件中读取节日信息
    void readFromFile(string filename) {
        // TODO: 从文件中读取节日信息
        ifstream infile(filename);
        string line;
        while (getline(infile, line)) {
            // 按照逗号分隔节日信息
            int pos1 = line.find(",");
            int pos2 = line.find(",", pos1 + 1);
            int pos3 = line.find(",", pos2 + 1);
            string name = line.substr(0, pos1);
            int month = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
            int day = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
            int type = stoi(line.substr(pos3 + 1));
            Festival f(name, month, day, type);
            festivals.push_back(f);
        }
        infile.close();
    }

    // 将节日信息保存到文件中
    void saveToFile(string filename) {
        // TODO: 将节日信息保存到文件中
        ofstream outfile(filename);
        for (const auto& f : festivals) {
            outfile << f.getName() << ","
                << f.getMonth() << ","
                << f.getDay() << ","
                << f.getType() << endl;
        }
        outfile.close();
    }

    // 增加一个节日
    void addFestival(Festival f) {
        // TODO: 增加一个节日
        if (!hasFestival(f.getName())) {
            festivals.push_back(f);
        }
    }

    // 删除一个节日
    void deleteFestival(string name) {
        // TODO: 删除一个节日
        for (auto it = festivals.begin(); it != festivals.end(); ++it) {
            if (it->getName() == name) {
                festivals.erase(it);
                break;
            }
        }
    }

    // 修改一个节日
    void modifyFestival(string name, Festival f) {
        // TODO: 修改一个节日
            bool found = false;
            for (int i = 0; i < festivals.size(); i++) {
                if (festivals[i].getName() == name) {
                    festivals[i] = f;
                    found = true;
                    break;
                }
            }
            if (!found) {
                throw runtime_error("Festival not found.");
        }
    }

    // 查询指定日期范围内的节日信息
    vector<Festival> queryFestivals(int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay, bool sortByDate = true) const {
        vector<Festival> result;
        // TODO: 查询指定日期范围内的节日信息
        // 如果sortByDate = true，则按日期升序排序，否则按节日名称升序排序
        vector<Festival> result;
        for (const auto& f : festivals) {
            // 判断节日日期是否在指定日期范围内
            if (f.getType() == 1 && f.getMonth() == startMonth && f.getDay() >= startDay ||
                f.getType() == 1 && f.getMonth() == endMonth && f.getDay() <= endDay ||
                f.getType() == 2 && f.getMonth() > startMonth && f.getMonth() < endMonth ||
                f.getType() == 2 && f.getMonth() == startMonth && f.getDay() >= startDay ||
                f.getType() == 2 && f.getMonth() == endMonth && f.getDay() <= endDay) {
                result.push_back(f);
            }
        }
        // 如果sortByDate = true，则按日期升序排序，否则按节日名称升序排序
        if (sortByDate) {
            sort(result.begin(), result.end(), [](const Festival& f1, const Festival& f2) {
                if (f1.getMonth() == f2.getMonth()) {
                    return f1.getDay() < f2.getDay();
                }
                return f1.getMonth() < f2.getMonth();
                });
        }
        else {
            sort(result.begin(), result.end(), [](const Festival& f1, const Festival& f2) {
                return f1.getName() < f2.getName();
                });
        }
        return result;
    }

    // 查询距离指定日期还有多少天的节日信息
    Festival queryFestivalByName(string name) const {
        // TODO: 查询距离指定日期还有多少天的节日信息
        for (const auto& f : festivals) {
            if (f.getName() == name) {
                return f;
            }
        }
        return Festival("", 0, 0, 0); // 找不到指定名称的节日，返回一个空的节日对象
    }
};

class CalendarView {
public:

    // 获取指定年月的第一天是星期几（0-6，0表示星期日）
    int getFirstDayOfWeek(int year, int month) const {
        tm timeinfo = {};
        timeinfo.tm_year = year - 1900;
        timeinfo.tm_mon = month - 1;
        timeinfo.tm_mday = 1;
        mktime(&timeinfo);
        return timeinfo.tm_wday;
    }

    // 获取指定年月的天数
    int getNumDaysOfMonth(int year, int month) const {
        if (month == 2 && (year % 4 == 0 && year % 100 != 0 || year % 400 == 0)) {
            return 29;
        }
        else {
            return daysInMonthTable[month - 1];
        }
    }
    // 显示日历和节日信息
    void showCalendar(int year, int month, const vector<Festival>& festivals) const {
        // TODO: 显示日历和节日信息
        // 计算指定年月的日历
        int first_day = getFirstDayOfWeek(year, month);
        int num_days = getNumDaysOfMonth(year, month);
        // 显示日历表头
        cout << "Sun\tMon\tTue\tWed\tThu\tFri\tSat" << endl;
        // 显示日历日期和节日信息
        int day = 1;
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 7; j++) {
                if (i == 0 && j < first_day || day > num_days) {
                    cout << "\t";
                }
                else {
                    // 显示日期
                    cout << day << "\t";
                    // 显示节日信息
                    for (const auto& f : festivals) {
                        if (f.isFestival(month, day)) {
                            cout << f.getName() << endl;
                        }
                    }
                    day++;
                }
            }
            cout << endl;
        }
    }

    // 显示节日查询结果
    void showFestivalResult(const vector<Festival>& festivals) const {
        // TODO: 显示节日查询结果
        // 显示查询结果表头
        cout << "Festival Name\tMonth\tDay\tType" << endl;
        // 显示查询结果
        for (const auto& f : festivals) {
            cout << f.getName() << "\t"
                << f.getMonth() << "\t"
                << f.getDay() << "\t"
                << (f.getType() == 1 ? "Fixed" : "Dynamic") << endl;
        }
    }

    // 添加节日
    Festival addFestival() const {
        // TODO: 添加节日
        // 显示一个表单，让用户输入新节日的信息
        string name;
        int month, day, type;

        cout << "Enter the festival name: ";
        cin >> name;
        cout << "Enter the festival month (1-12): ";
        cin >> month;
        cout << "Enter the festival day (1-31): ";
        cin >> day;
        cout << "Enter the festival type (1 for fixed, 2 for dynamic): ";
        cin >> type;

        // TODO: 显示一个表单，让用户输入新节日的信息
        // 调用FestivalManager类的addFestival()函数，将新节日添加到节日列表中


        Festival f(name, month, day, type);
        FestivalManager manager;
        manager.addFestival(f);
        // 显示一个提示消息，表示添加成功
        cout << "Festival \"" << name << "\" has been added." << endl;
        return f;
    }

    // 删除节日
    string deleteFestival() const {
        // TODO: 删除节日
        string name;
        cout << "Enter the festival name to delete: ";
        cin >> name;
        return name;
    }

    // 修改节日
    //void modifyFestival() const {
    //    // TODO: 修改节日
    //    // 输入要修改的节日名称
    //    string name = deleteFestival();
    //    // 查询指定名称的节日信息
    //    FestivalManager manager;
    //    Festival f = manager.queryFestivalByName(name);
    //    if (f.getName() != "") {
    //        // 显示一个表单，让用户输入新的节日信息
    //        // TODO: 显示一个表单，让用户输入新的节日信息
    //        // 调用FestivalManager类的modifyFestival()函数，将修改后的节日信息保存到节日列表中
    //        manager.modifyFestival(name, f);
    //        // 显示一个提示消息，表示修改成功
    //        cout << "Festival \"" << name << "\" has been modified." << endl;
    //    }
    //    else {
    //        cout << "Festival \"" << name << "\" not found." << endl;
    //    }
    //}
    Festival modifyFestival(const Festival& f) const {
        // 显示一个表单，让用户输入新的节日信息
        string name = f.getName();
        int month = f.getMonth();
        int day = f.getDay();
        int type = f.getType();
        cout << "Enter the new festival name (or press enter to keep \"" << name << "\"): ";
        string new_name;
        getline(cin, new_name);
        if (!new_name.empty()) {
            name = new_name;
        }
        cout << "Enter the new festival month (1-12, or press enter to keep " << month << "): ";
        string new_month;
        getline(cin, new_month);
        if (!new_month.empty()) {
            month = stoi(new_month);
        }
        cout << "Enter the new festival day (1-31, or press enter to keep " << day << "): ";
        string new_day;
        getline(cin, new_day);
        if (!new_day.empty()) {
            day = stoi(new_day);
        }
        cout << "Enter the new festival type (1 for fixed, 2 for dynamic, or press enter to keep " << type << "): ";
        string new_type;
        getline(cin, new_type);
        if (!new_type.empty()) {
            type = stoi(new_type);
        }
        Festival new_festival(name, month, day, type);
        return new_festival;
    }

    // 查询节日
    void queryFestival() const {
        // TODO: 查询节日
        // 显示一个表单，让用户输入查询范围
        int start_year, start_month, start_day, end_year, end_month, end_day;
        cout << "Enter the start year: ";
        cin >> start_year;
        cout << "Enter the start month (1-12): ";
        cin >> start_month;
        cout << "Enter the start day (1-31): ";
        cin >> start_day;
     }
};

class FestivalController {
private:
    FestivalManager manager; // 节日管理器
    CalendarView view; // 日历视图
public:
    // 启动节日管理系统
    void start() {
        // TODO: 启动节日管理系统，提供交互式的用户界面，实现节日信息的增加、删除、修改、查询和显示等功能。
    }
};

int main() {
    FestivalController controller;
    controller.start();
    return 0;
}