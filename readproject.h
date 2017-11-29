#ifndef READPROJECT_H
#define READPROJECT_H

#include <iostream>
#include <codecvt>
#include<string>
#include<vector>
#include<tuple>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/typeof/typeof.hpp>
using std::vector;
using std::string;
using std::tuple;
using std::exception;
using namespace boost::property_tree;
struct projectinfo;
struct modelinfo;
typedef int spindex;
enum eveltype
{
    rk1 = 1,
    rk2 = 2,
    rk4 = 4,
    rk8 = 8
};
struct modelinfo
{
    std::string _modelname;
    std::vector<std::string> _x_name;
    std::vector<std::string> _in_name;
    std::vector<std::string> _out_name;
    std::vector<std::string> _param_name;
    std::vector<std::tuple<std::string, std::string, std::string>> _linker;
    std::string _database; //数据具体怎么解析按理说应该是编库的人自己的问题
    std::string _dllpath;  //库加载路径
    std::string _funcname; //加载函数名称
};
struct projectinfo
{
    std::string _projectname;
    std::string _recorderpath;
    std::vector<modelinfo> _models;
};

void eraseStringHeadAndEnd(string &_string)
{
    uint32_t index = 0;
    for (auto word : _string)
    {
        if (word != '\t' && word != '\n' && word != ' ')
            break;
        index++;
    }
    _string.erase(_string.begin(), _string.begin() + index);
    index = 0;
    for (auto word = _string.rbegin(); word != _string.rend(); ++word)
    {
        if (*word != '\t' && *word != '\n' && *word != ' ')
            break;
        index++;
    }
    _string.erase(_string.end() - index, _string.end());
}

spindex findmodelindex(const string &_name, projectinfo *_themodel)
{
    spindex _index = 0;
    for (auto model_ : _themodel->_models)
    {
        if (_name == model_._modelname)
            return _index;
        ++_index;
    }
    return -1;
}
spindex findoutindex(const string &_name, modelinfo *_themodel)
{
    spindex _index = 0;
    for (auto name_ : _themodel->_out_name)
    {
        if (_name == name_)
            return _index;
        ++_index;
    }
    return -1;
}
spindex findinindex(const string &_name, modelinfo *_themodel)
{
    spindex _index = 0;
    for (auto name_ : _themodel->_in_name)
    {
        if (_name == name_)
            return _index;
        ++_index;
    }
    return -1;
}
//读取param
int readparam(const ptree &_ptree, vector<string> &_param)
{
    auto params_ = _ptree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "名称未定义"), vector<pair<char *, string>>()));
    for (auto param_ : params_)
    {
        if (string("subparam") == param_.first.data())
        {
            string str = param_.second.get<string>("<xmlattr>.name", "名称未定义");
            _param.push_back(str);
        }
    }
    return _param.size();
};
//读取x
int readx(const ptree &_ptree, vector<string> &_x)
{
    auto params_ = _ptree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "名称未定义"), vector<pair<char *, string>>()));
    for (auto param_ : params_)
    {
        if (string("substata") == param_.first.data())
        {
            string str = param_.second.get<string>("<xmlattr>.name", "名称未定义");
            _x.push_back(str);
        }
    }
    return _x.size();
};
//读取in
int readin(const ptree &_ptree, modelinfo &_model)
{
    auto params_ = _ptree.get_child("");
    auto &_in = _model._in_name;
    auto &_linkers = _model._linker;
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "名称未定义"), vector<pair<char *, string>>()));
    for (auto param_ : params_)
    {
        if (string("subin") == param_.first.data())
        {
            string instr_ = param_.second.get<string>("<xmlattr>.name","名称未定义");
            _in.push_back(instr_);
            auto linkers = param_.second.get_child("");
            for (auto link : linkers)
            {
                if (string("link") == link.first)
                {
                    string outmodelstr_ = link.second.get<string>("<xmlattr>.model", "名称未定义");
                    string outname_ = link.second.get<string>("", "未定义");
                    eraseStringHeadAndEnd(instr_);
                    eraseStringHeadAndEnd(outmodelstr_);
                    eraseStringHeadAndEnd(outname_);
                    _linkers.push_back(make_tuple(instr_, outmodelstr_, outname_));
                }
            }
        }
    }
    return _in.size();
};
//读取out
int readout(const ptree &_ptree, vector<string> &_out)
{
    auto params_ = _ptree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "名称未定义"), vector<pair<char *, string>>()));
    for (auto param_ : params_)
    {
        if (string("subout") == param_.first.data())
        {
            string str = param_.second.get<string>("<xmlattr>.name", "名称未定义");
            _out.push_back(str);
        }
    }
    return _out.size();
};
//读取dll路径
int readdll(const ptree &_ptree, modelinfo &_model)
{
    auto params_ = _ptree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "名称未定义"), vector<pair<char *, string>>()));
    for (auto param_ : params_)
    {
        if (string("path") == param_.first.data())
        {
            string str = param_.second.get<string>("", "");
            eraseStringHeadAndEnd(str);
            _model._dllpath = str;
        }
        if (string("name") == param_.first.data())
        {
            string str = param_.second.get<string>("", "");
            eraseStringHeadAndEnd(str);
            _model._funcname = (str);
        }
    }
    return 0;
}
//读取database路径
int readdatabase(const ptree &_ptree, modelinfo &_model)
{
    auto params_ = _ptree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "名称未定义"), vector<pair<char *, string>>()));
    for (auto param_ : params_)
    {
        if (string("data") == param_.first.data())
        {
            string strdata = param_.second.get<string>("", "未定义");
            eraseStringHeadAndEnd(strdata);
            _model._database = strdata;
        }
    }
    return _model._database.size();
}
//得到一个model
modelinfo make_model(const ptree &_ptree)
{
    modelinfo themodel_;
    themodel_._modelname = _ptree.get<string>("<xmlattr>.name", "notdef");
    auto submodels = _ptree.get_child("");
    for (auto submodel : submodels)
    {
        if (string("param") == submodel.first.data())
            readparam(submodel.second, themodel_._param_name);
        if (string("stata") == submodel.first.data())
            readx(submodel.second, themodel_._x_name);
        if (string("out") == submodel.first.data())
            readout(submodel.second, themodel_._out_name);
        if (string("in") == submodel.first.data())
            readin(submodel.second, themodel_);
        if (string("function") == submodel.first.data())
            readdll(submodel.second, themodel_);
        if (string("database") == submodel.first.data())
            readdatabase(submodel.second, themodel_);
    }
    return themodel_;
};

projectinfo *readprojectfromxml(const string &_path)
{
    ptree xmltree;
    ptree pt_empty;
    projectinfo *theinfp_ = nullptr;
    try
    {
#ifdef USE_UTF_8
        std::locale utf8Locale(std::locale(), new std::codecvt_utf8<wchar_t>());
        read_xml(_path, xmltree, 0, utf8Locale);
#else
        read_xml(_path, xmltree);
#endif
    }
    catch (exception &e)
    {
        printf("%s", e.what());
    }
    theinfp_ = new projectinfo();
    const ptree _models = xmltree.get_child("tarjet");
    for (auto &_model : _models)
    {
        //模型标签
        if (string("model") == _model.first.data())
        {
            theinfp_->_models.push_back(make_model(_model.second));
        }
    }
    return theinfp_;
};
#endif // READPROJECT_H
