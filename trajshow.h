#pragma once

#include <QtCore/QObject>

#include <nodes/NodeData.hpp>
#include <nodes/NodeDataModel.hpp>
#include<nodes/Connection.hpp>
#include <memory>
#include<QDebug>
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::PortType;
using QtNodes::PortIndex;

/// The class can potentially incapsulate any user data which
/// need to be transferred within the Node Editor graph
class MyNodeData : public NodeData
{
public:
  MyNodeData(NodeDataType _type={"noid","noname"})
  {
      m_datatype = _type;
  }
  NodeDataType
  type() const override
  {
    return m_datatype;
  }
private:
  NodeDataType m_datatype;
};

//------------------------------------------------------------------------------

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class NaiveDataModel : public NodeDataModel
{
  Q_OBJECT

public:

  virtual
  ~NaiveDataModel() {}
  NaiveDataModel()
  {
      m_modelname="The model";
      m_caption = "The caption";
//      std::shared_ptr<NodeData> in(new MyNodeData);
//      std::shared_ptr<NodeData> out(new MyNodeData);
//      m_indataname.push_back(in);
//      m_outdataname.push_back(out);
  }


public:

  QString
  caption() const override
  {
    return m_caption;
  }

  QString
  name() const override
  { return m_modelname; }

  std::unique_ptr<NodeDataModel>
  clone() const override
  {
      return std::make_unique<NaiveDataModel>();
  }

 public slots:
  void connectionhall(QtNodes::Connection& c, QPoint screenPos){
    QPoint p = screenPos;
//    qDebug()<<p;
  }

public:

  unsigned int
  nPorts(PortType portType) const override
  {
    unsigned int result = 1;

    switch (portType)
    {
      case PortType::In:
        result = m_indataname.size();
        break;

      case PortType::Out:
        result = m_outdataname.size();
        break;
      case PortType::None:
        break;
    }

    return result;
  }

  NodeDataType
  dataType(PortType portType,
           PortIndex portIndex) const override
  {
    switch (portType)
    {
      case PortType::In:
            return m_indataname[portIndex]->type();
      case PortType::Out:
            return m_outdataname[portIndex]->type();
      case PortType::None:
        break;
    }
    // FIXME: control may reach end of non-void function [-Wreturn-type]
    return NodeDataType();
  }

  std::shared_ptr<NodeData>
  outData(PortIndex port) override
  {
    return m_outdataname[port];
  }

  void
  setInData(std::shared_ptr<NodeData> _node, int _index) override
  {
        return;
  }

  QWidget *
  embeddedWidget() override { return nullptr; }
  int createData(PortType _type,QString _id, QString _name)
  {
      switch (_type)
      {
        case PortType::In:
          m_indataname.push_back(std::shared_ptr<NodeData>(new MyNodeData({_id,_name})));
          return m_indataname.size();
        case PortType::Out:
            m_outdataname.push_back(std::shared_ptr<NodeData>(new MyNodeData({_id,_name})));
            return m_outdataname.size();
        case PortType::None:
          return 0;
          break;
      }
      return -1;
  }
  void setModelName(QString _name)
  {
    m_modelname = (_name);
  }
  void setModelcaption(QString _name)
  {
    m_caption= _name;
  }
  int addData(PortType _datatype, QString _name)
  {
       return 0;
  }
private:
  QString m_modelname;
  QString m_caption;
  std::vector<std::shared_ptr<NodeData>> m_indataname;
  std::vector<std::shared_ptr<NodeData>> m_outdataname;
};

