#pragma once

#include<QString>
#include<string>

struct divide_info{
	std::string img_path;
	std::string img_name;
};

extern QString global_login_name;
extern QString global_login_institude;
extern QString global_login_career;

extern std::vector<divide_info> label_info_vec;
extern std::vector<divide_info> non_label_info_vec;
