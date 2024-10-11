//
// Created by DELL on 2024/8/28.
//

#include "Location.h"

std::vector<Location> CAUlocations = {
        {"王子楼", "男生宿舍，含第三食堂", ":/picture/WangZiLou.jpg"},
        {"CAU图书馆", "可借阅书籍、预定自习室", ":/picture/CAULibrary.jpg"},
        {"CAU三教", "多专业学生上课地点", ":/picture/CAUThirdBuilding.jpg"},
        {"CAU二教", "多专业学生上课地点", ":/picture/CAUSecondBuilding.jpg"},
        {"工学院", "该学院专业学生上课地点", ":/picture/EngineeringCollege.jpg"},
        {"水利与土木工程学院", "该学院专业学生上课地点", ":/picture/HydraulicCivilEngineeringCollege.jpg"},
        {"信息与电气工程学院", "该学院专业学生上课地点", ":/picture/ElectricalEngineeringCollege.jpg"},
        {"CAU一教", "多专业学生上课地点", ":/picture/CAUFirstBuilding.jpg"},
        {"CAU体育馆", "内有健身、乒乓球等运动场所", ":/picture/CAUGymnasium.jpg"},
        {"CAU运动场", "操场，可跑闪动", ":/picture/CAUStadium.jpg"},
        {"CAU公共澡堂", "内含男女浴室", ":/picture/CAUShower.jpg"},
        {"东门外卖柜", "取外卖的地方", ":/picture/EastGateTakeawayCabinet.jpg"},
        {"东门近邻宝", "取快递的地方", ":/picture/EastGateParcelLocker.jpg"},
        {"国际学院", "该学院专业学生上课地点", ":/picture/InternationalCollege.jpg"},
        {"篮球场", "打篮球的地方", ":/picture/BasketballCourt.jpg"},
        {"公主楼", "女生宿舍，内含食堂", ":/picture/PrincessBuilding.jpg"},
        {"人文与发展学院", "该学院专业学生上课地点", ":/picture/HumanitiesDevelopmentCollege.jpg"},
        {"商业街", "购买日常用品的地方", ":/picture/CommercialStreet.jpg"},
        {"研究生楼", "研究所宿舍，内含清真食堂", ":/picture/GraduateBuilding.jpg"},
        {"理学院", "该学院专业学生上课地点", ":/picture/ScienceCollege.jpg"},
        {"经济管理学院", "该学院专业学生上课地点", ":/picture/EconomicsManagementCollege.jpg"},
        {"CAU校医院", "看病的地方", ":/picture/CAUHospitall.jpg"},
        {"计网中心", "内含多个电脑教学室", ":/picture/NetworkCenter.jpg"},
        {"马锦明大楼", "多专业学生上课地点", ":/picture/MaJinMingBuilding.jpg"},
        {"东南门", "进出口", ":/picture/SouthEastGate.jpg"},
        {"北门", "进出口", ":/picture/NorthGate.jpg"},
        {"西门", "进出口", ":/picture/WestGate.jpg"}


};
QCollator collator;

bool compareLocations(const Location& loc1, const Location& loc2) {
    return collator.compare(loc1.name, loc2.name) < 0;
}

// 快速排序的分区函数
int partition(std::vector<Location>& locations, int low, int high) {
    Location pivot = locations[high];  // 选择最右侧的元素作为基准
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (compareLocations(locations[j], pivot)) {
            i++;
            std::swap(locations[i], locations[j]);
        }
    }
    std::swap(locations[i + 1], locations[high]);
    return i + 1;
}

// 快速排序算法
void quickSort(std::vector<Location>& locations, int low, int high) {
    if (low < high) {
        int pi = partition(locations, low, high);  // 获取分区点
        quickSort(locations, low, pi - 1);  // 递归排序左半部分
        quickSort(locations, pi + 1, high);  // 递归排序右半部分
    }
}

// 对 CAUlocations 进行排序的函数
void sortLocations(std::vector<Location>& locations) {
    quickSort(locations, 0, locations.size() - 1);
}
