#include "paint_scene.h"

namespace s21 {

PaintScene::PaintScene(QWidget *parent) : QCustomPlot(parent) {
  SetDefaultSettings();
}

void PaintScene::SetDefaultSettings() {
  this->setInteraction(QCP::iRangeZoom, true);
  this->setInteraction(QCP::iRangeDrag, true);
  this->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
  this->axisRect()->setRangeZoom(Qt::Horizontal);

  QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
  dateTicker->setDateTimeFormat("dd MMM yy");
  this->xAxis->setTicker(dateTicker);

  this->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
  this->xAxis->setLabel("Date");

  this->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
  this->yAxis->setLabel("Price");

  this->xAxis2->setVisible(true);
  this->yAxis2->setVisible(true);
  this->xAxis2->setTicks(false);
  this->yAxis2->setTicks(false);
  this->xAxis2->setTickLabels(false);
  this->yAxis2->setTickLabels(false);

  this->legend->setVisible(true);
  this->legend->setBrush(QColor(255, 255, 255, 150));
}

void PaintScene::AddDotedGraph(const QVector<QCPGraphData> &timeData) {
  this->addGraph();
  this->graph()->setPen(QColor(0, 0, 0, 255));
  this->graph()->setLineStyle(QCPGraph::lsNone);
  this->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
  this->graph()->setName("Native data");
  this->graph()->data()->set(timeData);
  this->xAxis->setRange(timeData[0].key - RECTIFICATION_FACTOR,
                        timeData.last().key + RECTIFICATION_FACTOR);
  this->rescaleAxes();
  this->replot();
}

QColor PaintScene::GetRandomColor() {
  QColor result;
  result.setRgb(s21::Generator::GetRandomNumber(),
                s21::Generator::GetRandomNumber(),
                s21::Generator::GetRandomNumber());
  return result;
}

QCPGraph *PaintScene::AddLinesGraph(const QVector<QCPGraphData> &timeData,
                                    const QString &graph_name) {
  if (graph_counter_ >= MAX_GRAPH_VALUE) return nullptr;

  QCPGraph *graph = this->addGraph();
  this->graph()->setLineStyle(QCPGraph::lsLine);
  this->graph()->setPen(QPen(GetRandomColor()));
  this->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
  this->graph()->data()->set(timeData);
  this->graph()->setName(graph_name);

  this->xAxis->setRange(timeData[0].key - RECTIFICATION_FACTOR,
                        timeData.last().key + RECTIFICATION_FACTOR);
  this->rescaleAxes();
  this->replot();
  graphs_.push_back(graph);
  graph_counter_++;
  return graph;
}

bool PaintScene::removeGraph(QCPGraph *graph) {
  graph_counter_--;
  return QCustomPlot::removeGraph(graph);
}

int PaintScene::clearGraphs() {
  graph_counter_ = 0;
  graphs_.clear();
  QCustomPlot::clearGraphs();
  return graph_counter_;
}

}  // namespace s21
