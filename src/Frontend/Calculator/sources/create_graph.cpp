#include "../headers/mainwindow.h"
#include "ui_mainwindow.h"

#define AMOUNT_STEP 100001

void MainWindow::on_button_graph_create_clicked() {
  check_axis_value();
  ui->graph_show->clearGraphs();
  ui->graph_show->replot();
  ui->output_line->clear();
  char *input_array = get_input_array();
  int output = input_array_validation(input_array);
  if (output == OK) {
    int amount_tokens = 0;
    token *reverse_polish_notation_array =
        reverse_polish_notation(input_array, &amount_tokens);
    if (reverse_polish_notation_array != nullptr) {
      double x_start = ui->spin_coord_x_start->value();
      double x_end = ui->spin_coord_x_end->value();
      double step = (x_end - x_start) / (AMOUNT_STEP - 1);
      QVector<double> x(AMOUNT_STEP), y(AMOUNT_STEP);
      double result = 0.0;
      int i = 0;
      for (double x_value = x_start; x_value <= x_end && output == OK;
           x_value += step, ++i) {
        token *tmp = copy_array(reverse_polish_notation_array, amount_tokens);
        output = calculation(tmp, amount_tokens, x_value, &result);
        if (output == OK) {
          x[i] = x_value;
          y[i] = result;
        }
        free(tmp);
        tmp = nullptr;
      }
      if (output == OK) {
        plot_graph(x, y);
      }
      free(reverse_polish_notation_array);
      reverse_polish_notation_array = nullptr;
    } else {
      output = SYNTAX_ERROR;
    }
  }
  if (output != OK) {
    ui->output_line->setText(SYNTAX_ERROR_LINE);
  }
  free_pointer(input_array);
}

token *MainWindow::copy_array(token *input_array, int amount_tokens) {
  token *output_array = (token *)calloc(amount_tokens, sizeof(token));
  for (int i = 0; i < amount_tokens; ++i) {
    output_array[i] = input_array[i];
  }
  return output_array;
}

void MainWindow::on_button_graph_clear_clicked() {
  ui->graph_show->clearGraphs();
  ui->graph_show->replot();
}

void MainWindow::plot_graph(const QVector<double> x, const QVector<double> y) {
  ui->graph_show->addGraph();
  set_axis_value();
  ui->graph_show->graph(0)->setData(x, y);
  graph_settings();
  ui->graph_show->replot();
}

void MainWindow::graph_settings() {
  ui->graph_show->graph(0)->setPen(QColor(50, 50, 50, 255));
  ui->graph_show->graph(0)->setLineStyle(QCPGraph::lsNone);
  ui->graph_show->graph(0)->setScatterStyle(
      QCPScatterStyle(QCPScatterStyle::ssCircle, 1));
  ui->graph_show->setInteraction(QCP::iRangeZoom, true);
  ui->graph_show->setInteraction(QCP::iRangeDrag, true);
}
