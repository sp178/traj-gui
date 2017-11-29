#include <QtWidgets/QApplication>

#include <nodes/NodeData.hpp>
#include <nodes/FlowScene.hpp>
#include <nodes/FlowView.hpp>
#include <nodes/DataModelRegistry.hpp>
#include <nodes/ConnectionStyle.hpp>
#include<nodes/Node.hpp>
#include"modelbase.h"
#include"trajshow.h"
using QtNodes::DataModelRegistry;
using QtNodes::FlowScene;
using QtNodes::FlowView;
using QtNodes::ConnectionStyle;
using QtNodes::Node;

static std::shared_ptr<DataModelRegistry>
registerDataModels()
{
  auto ret = std::make_shared<DataModelRegistry>();

  ret->registerModel<NaiveDataModel>();
  /*
     We could have more models registered.
     All of them become items in the context meny of the scene.

     ret->registerModel<AnotherDataModel>();
     ret->registerModel<OneMoreDataModel>();

   */

  return ret;
}


static
void
setStyle()
{
  ConnectionStyle::setConnectionStyle(
    R"(
  {
    "ConnectionStyle": {
      "UseDataDefinedColors": true
    }
  }
  )");
}


//------------------------------------------------------------------------------

int
main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  setStyle();

  FlowScene scene(registerDataModels());

  FlowView view(&scene);

  projectinfo *info = nullptr;
  info = readprojectfromxml("/home/sp178/project/trajproject/data/simulation.xml");


  vector<Node*> nodes;
  for(int index = 0;index<info->_models.size();++index)
  {
      std::unique_ptr<NaiveDataModel> mymodel(new NaiveDataModel);
      bool is =QObject::connect(&scene, SIGNAL(connectionHovered(QtNodes::Connection& , QPoint)),
                       mymodel.get(),SLOT(connectionhall(QtNodes::Connection& c, QPoint screenPos)));
      for(auto inname: info->_models[index]._in_name)
        mymodel->createData(PortType::In,"in",inname.c_str());
      for(auto outname: info->_models[index]._out_name)
        mymodel->createData(PortType::Out,"in",outname.c_str());

      mymodel->setModelcaption(info->_models[index]._modelname.c_str());
      auto& node = scene.createNode(std::move(mymodel));
      nodes.push_back(&node);
      scene.setNodePosition(node,{350*index,0});
  }

  int index = 0;
  for(auto model:info->_models)
  {
      for(auto link:model._linker)
      {
          int outmodelindex = findmodelindex(std::get<1>(link),info);
          int outdataindex =  findoutindex(std::get<2>(link),&info->_models[outmodelindex]);
          int indataindex = findinindex(std::get<0>(link),&info->_models[index]);
          scene.createConnection(*nodes[index],indataindex,
                                 *nodes[outmodelindex],outdataindex);
      }
      index++;
  }

  view.setWindowTitle("Node-based flow editor");
  view.resize(1920/1.5, 1080/1.5);
  view.show();

  return app.exec();
}
