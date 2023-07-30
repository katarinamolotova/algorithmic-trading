#ifndef PAINT_SCENE_H
#define PAINT_SCENE_H

#define MAX_GRAPH_VALUE 5
#define RECTIFICATION_FACTOR 86400

#include <QWidget>

#include "helper/generator.h"
#include "qcustomplot.h"

namespace s21 {

class PaintScene : public QCustomPlot {
 public:
  explicit PaintScene(QWidget* parent = nullptr);
  QCPGraph* AddLinesGraph(const QVector<QCPGraphData>& timeData,
                          const QString& graph_name);
  void AddDotedGraph(const QVector<QCPGraphData>& timeData);
  bool removeGraph(QCPGraph* graph) override;
  int clearGraphs() override;

 private:
  QVector<QCPGraph*> graphs_;
  int graph_counter_ = 0;
  void SetDefaultSettings();
  static QColor GetRandomColor();
};

}  // namespace s21

#endif  // PAINT_SCENE_H
