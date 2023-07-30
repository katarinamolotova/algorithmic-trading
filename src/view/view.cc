#include "view.h"

namespace s21 {

view::view(s21::Controller *controller, QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::view) {
  controller_ = controller;
  SetStartSetting();
}

view::~view() {
  delete controller_;
  delete ui_;
}

void view::SetStartSetting() {
  ui_->setupUi(this);
  this->EnableButtons(false);
  ConnectionButtons();
  ui_->dateTimeEdit->setEnabled(false);
  ui_->ok_button->setVisible(false);
  ui_->calendarWidget->setVisible(false);
  ui_->timeEdit->setVisible(false);
  ui_->set_button->setEnabled(false);
}

void view::EnableButtons(bool flag) {
  ui_->get_price->setEnabled(flag);
  ui_->print_graph->setEnabled(flag);
}

void view::ChoseDateFromCalendar(const QDate &date) {
  ui_->dateTimeEdit->setDate(ui_->calendarWidget->selectedDate());
  ui_->calendarWidget->setVisible(false);
  ui_->timeEdit->setVisible(true);
  ui_->ok_button->setVisible(true);
}

void view::EnableCommonButtons(bool enable) {
  ui_->set_button->setVisible(enable);
  ui_->load_data->setEnabled(enable);
  ui_->clear_scene->setEnabled(enable);
  ui_->tabWidget->setEnabled(enable);
}

void view::SetDateTimeRange() {
  EnableCommonButtons(false);
  ui_->calendarWidget->setVisible(true);
  ui_->dateTimeEdit->setVisible(false);
}

void view::ConfirmTimeEntry() {
  ui_->dateTimeEdit->setTime(ui_->timeEdit->time());
  ui_->timeEdit->setVisible(false);
  ui_->ok_button->setVisible(false);
  ui_->dateTimeEdit->setVisible(true);
  ui_->calendarWidget->setVisible(false);
  if (dataset_loaded) {
    ui_->get_price->setEnabled(true);
  }
  ui_->check_box_time->setChecked(true);
  EnableCommonButtons(true);
}

void view::SetTheMaxDegreeOfThePolynomial() {
  if (ui_->tabWidget->currentIndex() == kApproximation) {
    controller_->GetData(kAll).size();
    ui_->degree_of_polynom->setMaximum(
        static_cast<int>(controller_->GetData(kAll).size() - 1));
  }
}

void view::ChangedTab() {
  ui_->check_box_time->setChecked(false);
  ui_->widget->clearGraphs();
  ui_->widget->replot();
  ClearParametersBox(false);
  EnableButtons(false);
  if (dataset_loaded) {
    SetTimeRange();
    ui_->print_graph->setEnabled(true);
    PrintOriginPointsFromFile();
    SetTheMaxDegreeOfThePolynomial();
  }
}

void view::GetPrice() {
  double result;
  QDateTime data_time = ui_->dateTimeEdit->dateTime();
  if (ui_->tabWidget->currentIndex() == kInterpolation) {
    controller_->Refresh(kInterpolation);
    result = controller_->GetValue(kInterpolation, data_time);
    ui_->label_price->setText(QString::number(result));
  } else if (ui_->tabWidget->currentIndex() == kApproximation) {
    controller_->Refresh(kApproximation);
    result = controller_->GetValue(kApproximation, data_time,
                                   ui_->number_of_points->value());
    ui_->label_price->setText(QString::number(result));
  }
}

void view::PrintGraph() {
  int points = ui_->number_of_points->value();
  QVector<QCPGraphData> data;
  if (ui_->tabWidget->currentIndex() == kInterpolation) {
    controller_->Refresh(kInterpolation);
    data = controller_->GetData(kInterpolation, points);
  } else if (ui_->tabWidget->currentIndex() == kApproximation) {
    controller_->Refresh(kApproximation);
    data = controller_->GetData(kApproximation, points,
                                ui_->degree_of_polynom->value(),
                                ui_->number_of_days->value());
  }
  PrintGraphWithLines(data,
                      PrepareAnswer(points, ui_->tabWidget->currentIndex()));
}

QString view::PrepareAnswer(int points, int type) {
  QString result = QString::number(points);
  if (type == kApproximation) {
    result += ", polynomial degree - " +
              QString::number(ui_->degree_of_polynom->value());
  }
  return "Graph: number_of_points - " + result;
}

void view::SetTimeRange() {
  double first_date = controller_->GetData(kAll).first().key;
  ui_->calendarWidget->setMinimumDate(
      QDateTime::fromSecsSinceEpoch(static_cast<qint64>(first_date)).date());
  SetMaxDate(ui_->tabWidget->currentIndex());
}

void view::SetMaxDate(int type) {
  if (type == kApproximation) {
    ui_->calendarWidget->setMaximumDate(QDate::currentDate());
  } else if (type == kInterpolation) {
    double pre_last_date =
        controller_->GetData(kAll).last().key - RECTIFICATION_FACTOR;
    ui_->calendarWidget->setMaximumDate(
        QDateTime::fromSecsSinceEpoch(static_cast<qint64>(pre_last_date))
            .date());
  }
}

void view::PrintGraphWithLines(const QVector<QCPGraphData> &data,
                               const QString &graph_name) {
  if (ui_->tabWidget->currentIndex() == kApproximation &&
      (last_numbers_of_the_days != ui_->number_of_days->value())) {
    Clear();
    last_numbers_of_the_days = ui_->number_of_days->value();
  }
  ui_->widget->AddLinesGraph(data, graph_name);
  graph_counter_++;
}

void view::ClearParametersBox(bool success) {
  if (!success) {
    ui_->number_of_points->setMinimum(0);
    ui_->number_of_points->setValue(0);
    ui_->degree_of_polynom->setValue(0);
    ui_->number_of_days->setValue(0);
  }
}

void view::ReConfigView(bool success) {
  ClearParametersBox(success);
  dataset_loaded = success;
  ui_->check_box_data->setChecked(success);
  ui_->set_button->setEnabled(success);
}

void view::SetFileNameOnInterFace(const QString &fileName) {
  std::string tmp_file_name = fileName.toStdString();
  auto it = tmp_file_name.find_last_of('/');
  ui_->file_name->setText(QString::fromStdString(
      tmp_file_name.substr(it + 1, tmp_file_name.back())));
}

void view::LoadDataFromFile() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Choose file"), QDir::currentPath(), tr("*.csv"));

  SetFileNameOnInterFace(fileName);
  bool success = controller_->LoadDataFromFile(fileName.toStdString());
  ReConfigView(success);

  if (!success) {
    QMessageBox::critical(this, "Loading datasets", "Something went wrong!");
    EnableButtons(false);
    ui_->widget->clearGraphs();
    ui_->widget->replot();
  } else {
    ui_->print_graph->setEnabled(true);
    PrintOriginPointsFromFile();
    SetTimeRange();
  }
}

void view::Clear() {
  ui_->widget->clearGraphs();
  ui_->widget->replot();
  ui_->dateTimeEdit->clear();
  if (ui_->tabWidget->currentIndex() == kInterpolation) {
    ui_->label_price->clear();
  } else if (ui_->tabWidget->currentIndex() == kApproximation) {
    ui_->label_price->clear();
  }
  ui_->check_box_time->setChecked(false);
  EnableButtons(false);
  if (dataset_loaded) {
    PrintOriginPointsFromFile();
    ui_->print_graph->setEnabled(true);
  }
}

void view::ConnectionButtons() {
  connect(ui_->print_graph, SIGNAL(clicked()), this, SLOT(PrintGraph()));
  connect(ui_->load_data, SIGNAL(clicked()), this, SLOT(LoadDataFromFile()));
  connect(ui_->get_price, SIGNAL(clicked()), this, SLOT(GetPrice()));
  connect(ui_->clear_scene, SIGNAL(clicked()), this, SLOT(Clear()));
  connect(ui_->ok_button, SIGNAL(clicked()), this, SLOT(ConfirmTimeEntry()));
  connect(ui_->set_button, SIGNAL(clicked()), this, SLOT(SetDateTimeRange()));
  connect(ui_->tabWidget, SIGNAL(currentChanged(int)), this,
          SLOT(ChangedTab()));
  connect(ui_->calendarWidget, SIGNAL(activated(QDate)), this,
          SLOT(ChoseDateFromCalendar(const QDate &)));
}

void view::SetMinMaxPointsOnGraph(QVector<QCPGraphData> &data) {
  ui_->number_of_points->setMinimum(static_cast<int>(data.size()));
  ui_->number_of_points->setMaximum(static_cast<int>(data.size()) *
                                    AUGMENTATION_FACTOR);
}

void view::PrintOriginPointsFromFile() {
  ui_->widget->clearGraphs();
  ui_->widget->replot();
  QVector<QCPGraphData> data = controller_->GetData(kAll);
  SetMinMaxPointsOnGraph(data);
  ui_->widget->AddDotedGraph(data);
}

}  // namespace s21
