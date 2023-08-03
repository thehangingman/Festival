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
    string name; // ��������
    int month; // �����·�
    int day; // ��������
    int type; // �������ͣ�1��ʾ�̶����ڣ�2��ʾ��̬����
public:
    // ���캯��
    Festival(string name, int month, int day, int type) {
        this->name = name;
        this->month = month;
        this->day = day;
        this->type = type;
    }

    // ��ȡ��������
    string getName() const {
        return name;
    }

    // ��ȡ�����·�
    int getMonth() const {
        return month;
    }

    // ��ȡ��������
    int getDay() const {
        return day;
    }

    // ��ȡ��������
    int getType() const {
        return type;
    }

    // �ж��Ƿ�Ϊָ�����ڵĽ���
    bool isFestival(int month, int day) const {
        if (this->month == month && this->day == day) {
            return true;
        }
        return false;
    }

    // �������ָ�����ڻ��ж�����
    int daysTo(int month, int day) const {
        int days = 0;
        // TODO: �������ָ�����ڻ��ж�����
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        int year = timeinfo->tm_year + 1900; // �����Ҫ���� 1900
        int month_today = timeinfo->tm_mon + 1;
        int day_today = timeinfo->tm_mday;
        int days = 0;
        if (month < month_today || (month == month_today && day < day_today)) {
            // ���ָ�������ڽ���֮ǰ�����������һ���ָ�����յ�����
            tm timeinfo_festival = *timeinfo;
            timeinfo_festival.tm_year = year;
            timeinfo_festival.tm_mon = month - 1;
            timeinfo_festival.tm_mday = day;
            time_t festival = mktime(&timeinfo_festival);
            tm* timeinfo_next_year = localtime(&festival);
            timeinfo_next_year->tm_year++; // ��һ��
            time_t next_year = mktime(timeinfo_next_year);
            days = (next_year - now) / (24 * 3600); // ��������
        }
        else {
            // ��������������ָ�����յ�����
            tm timeinfo_festival = *timeinfo;
            timeinfo_festival.tm_year = year - 1900;
            timeinfo_festival.tm_mon = month - 1;
            timeinfo_festival.tm_mday = day;
            time_t festival = mktime(&timeinfo_festival);
            days = (festival - now) / (24 * 3600); // ��������
        }
        return days;

    }
};

class FestivalManager {
private:
    vector<Festival> festivals; // �����б�
public:
    bool hasFestival(string name) const {
        for (const auto& f : festivals) {
            if (f.getName() == name) {
                return true;
            }
        }
        return false;
    }
    // ���ļ��ж�ȡ������Ϣ
    void readFromFile(string filename) {
        // TODO: ���ļ��ж�ȡ������Ϣ
        ifstream infile(filename);
        string line;
        while (getline(infile, line)) {
            // ���ն��ŷָ�������Ϣ
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

    // ��������Ϣ���浽�ļ���
    void saveToFile(string filename) {
        // TODO: ��������Ϣ���浽�ļ���
        ofstream outfile(filename);
        for (const auto& f : festivals) {
            outfile << f.getName() << ","
                << f.getMonth() << ","
                << f.getDay() << ","
                << f.getType() << endl;
        }
        outfile.close();
    }

    // ����һ������
    void addFestival(Festival f) {
        // TODO: ����һ������
        if (!hasFestival(f.getName())) {
            festivals.push_back(f);
        }
    }

    // ɾ��һ������
    void deleteFestival(string name) {
        // TODO: ɾ��һ������
        for (auto it = festivals.begin(); it != festivals.end(); ++it) {
            if (it->getName() == name) {
                festivals.erase(it);
                break;
            }
        }
    }

    // �޸�һ������
    void modifyFestival(string name, Festival f) {
        // TODO: �޸�һ������
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

    // ��ѯָ�����ڷ�Χ�ڵĽ�����Ϣ
    vector<Festival> queryFestivals(int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay, bool sortByDate = true) const {
        vector<Festival> result;
        // TODO: ��ѯָ�����ڷ�Χ�ڵĽ�����Ϣ
        // ���sortByDate = true���������������򣬷��򰴽���������������
        vector<Festival> result;
        for (const auto& f : festivals) {
            // �жϽ��������Ƿ���ָ�����ڷ�Χ��
            if (f.getType() == 1 && f.getMonth() == startMonth && f.getDay() >= startDay ||
                f.getType() == 1 && f.getMonth() == endMonth && f.getDay() <= endDay ||
                f.getType() == 2 && f.getMonth() > startMonth && f.getMonth() < endMonth ||
                f.getType() == 2 && f.getMonth() == startMonth && f.getDay() >= startDay ||
                f.getType() == 2 && f.getMonth() == endMonth && f.getDay() <= endDay) {
                result.push_back(f);
            }
        }
        // ���sortByDate = true���������������򣬷��򰴽���������������
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

    // ��ѯ����ָ�����ڻ��ж�����Ľ�����Ϣ
    Festival queryFestivalByName(string name) const {
        // TODO: ��ѯ����ָ�����ڻ��ж�����Ľ�����Ϣ
        for (const auto& f : festivals) {
            if (f.getName() == name) {
                return f;
            }
        }
        return Festival("", 0, 0, 0); // �Ҳ���ָ�����ƵĽ��գ�����һ���յĽ��ն���
    }
};

class CalendarView {
public:

    // ��ȡָ�����µĵ�һ�������ڼ���0-6��0��ʾ�����գ�
    int getFirstDayOfWeek(int year, int month) const {
        tm timeinfo = {};
        timeinfo.tm_year = year - 1900;
        timeinfo.tm_mon = month - 1;
        timeinfo.tm_mday = 1;
        mktime(&timeinfo);
        return timeinfo.tm_wday;
    }

    // ��ȡָ�����µ�����
    int getNumDaysOfMonth(int year, int month) const {
        if (month == 2 && (year % 4 == 0 && year % 100 != 0 || year % 400 == 0)) {
            return 29;
        }
        else {
            return daysInMonthTable[month - 1];
        }
    }
    // ��ʾ�����ͽ�����Ϣ
    void showCalendar(int year, int month, const vector<Festival>& festivals) const {
        // TODO: ��ʾ�����ͽ�����Ϣ
        // ����ָ�����µ�����
        int first_day = getFirstDayOfWeek(year, month);
        int num_days = getNumDaysOfMonth(year, month);
        // ��ʾ������ͷ
        cout << "Sun\tMon\tTue\tWed\tThu\tFri\tSat" << endl;
        // ��ʾ�������ںͽ�����Ϣ
        int day = 1;
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 7; j++) {
                if (i == 0 && j < first_day || day > num_days) {
                    cout << "\t";
                }
                else {
                    // ��ʾ����
                    cout << day << "\t";
                    // ��ʾ������Ϣ
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

    // ��ʾ���ղ�ѯ���
    void showFestivalResult(const vector<Festival>& festivals) const {
        // TODO: ��ʾ���ղ�ѯ���
        // ��ʾ��ѯ�����ͷ
        cout << "Festival Name\tMonth\tDay\tType" << endl;
        // ��ʾ��ѯ���
        for (const auto& f : festivals) {
            cout << f.getName() << "\t"
                << f.getMonth() << "\t"
                << f.getDay() << "\t"
                << (f.getType() == 1 ? "Fixed" : "Dynamic") << endl;
        }
    }

    // ��ӽ���
    Festival addFestival() const {
        // TODO: ��ӽ���
        // ��ʾһ���������û������½��յ���Ϣ
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

        // TODO: ��ʾһ���������û������½��յ���Ϣ
        // ����FestivalManager���addFestival()���������½�����ӵ������б���


        Festival f(name, month, day, type);
        FestivalManager manager;
        manager.addFestival(f);
        // ��ʾһ����ʾ��Ϣ����ʾ��ӳɹ�
        cout << "Festival \"" << name << "\" has been added." << endl;
        return f;
    }

    // ɾ������
    string deleteFestival() const {
        // TODO: ɾ������
        string name;
        cout << "Enter the festival name to delete: ";
        cin >> name;
        return name;
    }

    // �޸Ľ���
    //void modifyFestival() const {
    //    // TODO: �޸Ľ���
    //    // ����Ҫ�޸ĵĽ�������
    //    string name = deleteFestival();
    //    // ��ѯָ�����ƵĽ�����Ϣ
    //    FestivalManager manager;
    //    Festival f = manager.queryFestivalByName(name);
    //    if (f.getName() != "") {
    //        // ��ʾһ���������û������µĽ�����Ϣ
    //        // TODO: ��ʾһ���������û������µĽ�����Ϣ
    //        // ����FestivalManager���modifyFestival()���������޸ĺ�Ľ�����Ϣ���浽�����б���
    //        manager.modifyFestival(name, f);
    //        // ��ʾһ����ʾ��Ϣ����ʾ�޸ĳɹ�
    //        cout << "Festival \"" << name << "\" has been modified." << endl;
    //    }
    //    else {
    //        cout << "Festival \"" << name << "\" not found." << endl;
    //    }
    //}
    Festival modifyFestival(const Festival& f) const {
        // ��ʾһ���������û������µĽ�����Ϣ
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

    // ��ѯ����
    void queryFestival() const {
        // TODO: ��ѯ����
        // ��ʾһ���������û������ѯ��Χ
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
    FestivalManager manager; // ���չ�����
    CalendarView view; // ������ͼ
public:
    // �������չ���ϵͳ
    void start() {
        // TODO: �������չ���ϵͳ���ṩ����ʽ���û����棬ʵ�ֽ�����Ϣ�����ӡ�ɾ�����޸ġ���ѯ����ʾ�ȹ��ܡ�
    }
};

int main() {
    FestivalController controller;
    controller.start();
    return 0;
}