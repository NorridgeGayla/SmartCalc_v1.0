#include "./headers/mainwindow.h"

#include <math.h>

#include <QRegularExpressionValidator>
#include <QVector>

#include "ui_mainwindow.h"

#define EPS 1e-07

#define DEFAULT_SPIN_VALUE 10

#define DEPOSIT_PROCENT_CB "7.5"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  start_settings();
  connect_signals();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::start_settings() {
  set_default_spin_x_coord();
  ui->graph_show->setBackground(QBrush(QColor(243, 244, 251)));
  set_axis_value();
  set_validators();
  ui->annuitet->setChecked(true);
  QDate date_today = QDate::currentDate();
  ui->deposit_start_term->setDate(date_today);
  ui->deposit_date_replanishment->setDate(date_today);
  ui->deposit_date_withdraw->setDate(date_today);
  ui->deposit_procent_CB->setText(DEPOSIT_PROCENT_CB);
}

void MainWindow::set_validators() {
  set_x_value_validator();
  set_credit_calc_validator();
  set_deposit_calc_validator();
}

void MainWindow::set_x_value_validator() {
  QRegularExpression rx("[-]?([0-9]*)[.]?[0-9]*");
  QValidator *validator = new QRegularExpressionValidator(rx, this);
  ui->x_value->setValidator(validator);
}

void MainWindow::set_credit_calc_validator() {
  QRegularExpression rx("[0-9]*");
  QValidator *validator1 = new QRegularExpressionValidator(rx, this);
  ui->credit_sum->setValidator(validator1);
  ui->credit_term->setValidator(validator1);
  rx.setPattern("[0-9]*[.]?[0-9]*");
  QValidator *validator2 = new QRegularExpressionValidator(rx, this);
  ui->credir_procent->setValidator(validator2);
}

void MainWindow::set_deposit_calc_validator() {
  QRegularExpression rx("[0-9]*");
  QValidator *validator1 = new QRegularExpressionValidator(rx, this);
  ui->deposit_sum->setValidator(validator1);
  ui->deposit_term->setValidator(validator1);
  ui->deposit_sum_replanishment->setValidator(validator1);
  ui->deposit_sum_withdraw->setValidator(validator1);
  rx.setPattern("[0-9]*[.]?[0-9]*");
  QValidator *validator2 = new QRegularExpressionValidator(rx, this);
  ui->deposit_procent->setValidator(validator2);
  ui->deposit_procent_CB->setValidator(validator2);
}

void MainWindow::connect_signals() {
  connect(ui->button_0, SIGNAL(clicked()), this, SLOT(simple_button()));
  connect(ui->button_1, SIGNAL(clicked()), this, SLOT(simple_button()));
  connect(ui->button_2, SIGNAL(clicked()), this, SLOT(simple_button()));
  connect(ui->button_3, SIGNAL(clicked()), this, SLOT(simple_button()));
  connect(ui->button_4, SIGNAL(clicked()), this, SLOT(simple_button()));
  connect(ui->button_5, SIGNAL(clicked()), this, SLOT(simple_button()));
  connect(ui->button_6, SIGNAL(clicked()), this, SLOT(simple_button()));
  connect(ui->button_7, SIGNAL(clicked()), this, SLOT(simple_button()));
  connect(ui->button_8, SIGNAL(clicked()), this, SLOT(simple_button()));
  connect(ui->button_9, SIGNAL(clicked()), this, SLOT(simple_button()));
  connect(ui->button_x, SIGNAL(clicked()), this, SLOT(simple_button()));
  connect(ui->button_e, SIGNAL(clicked()), this, SLOT(simple_button()));
  connect(ui->button_sum, SIGNAL(clicked()), this, SLOT(simple_button()));
  connect(ui->button_sub, SIGNAL(clicked()), this, SLOT(simple_button()));
  connect(ui->button_mul, SIGNAL(clicked()), this, SLOT(simple_button()));
  connect(ui->button_div, SIGNAL(clicked()), this, SLOT(simple_button()));
  connect(ui->button_dot, SIGNAL(clicked()), this, SLOT(simple_button()));
  connect(ui->button_pow, SIGNAL(clicked()), this, SLOT(simple_button()));
  connect(ui->button_mod, SIGNAL(clicked()), this, SLOT(simple_button()));
  connect(ui->button_right_bracket, SIGNAL(clicked()), this,
          SLOT(simple_button()));
  connect(ui->button_left_bracket, SIGNAL(clicked()), this,
          SLOT(simple_button()));
  connect(ui->button_sqrt, SIGNAL(clicked()), this, SLOT(function_button()));
  connect(ui->button_log, SIGNAL(clicked()), this, SLOT(function_button()));
  connect(ui->button_ln, SIGNAL(clicked()), this, SLOT(function_button()));
  connect(ui->button_sin, SIGNAL(clicked()), this, SLOT(function_button()));
  connect(ui->button_cos, SIGNAL(clicked()), this, SLOT(function_button()));
  connect(ui->button_tan, SIGNAL(clicked()), this, SLOT(function_button()));
  connect(ui->button_asin, SIGNAL(clicked()), this, SLOT(function_button()));
  connect(ui->button_acos, SIGNAL(clicked()), this, SLOT(function_button()));
  connect(ui->button_atan, SIGNAL(clicked()), this, SLOT(function_button()));
}

void MainWindow::simple_button() {
  QPushButton *button = (QPushButton *)sender();
  QString text_prev = ui->input_line->toPlainText();
  ui->input_line->setText(text_prev + button->text());
}

void MainWindow::function_button() {
  QPushButton *button = (QPushButton *)sender();
  QString text_prev = ui->input_line->toPlainText();
  ui->input_line->setText(text_prev + button->text() + '(');
}

void MainWindow::on_button_pi_clicked() {
  QString text_prev = ui->input_line->toPlainText();
  ui->input_line->setText(text_prev + "pi");
}

void MainWindow::on_button_del_clicked() {
  QString text_prev = ui->input_line->toPlainText();
  text_prev.remove(text_prev.size() - 1, 1);
  ui->input_line->setText(text_prev);
}

void MainWindow::on_button_AC_clicked() {
  ui->input_line->clear();
  ui->output_line->clear();
}

void MainWindow::on_button_default_coord_x_clicked() {
  set_default_spin_x_coord();
}

void MainWindow::set_default_spin_x_coord() {
  ui->spin_coord_x_start->setValue(-DEFAULT_SPIN_VALUE);
  ui->spin_coord_x_end->setValue(DEFAULT_SPIN_VALUE);
}

char *MainWindow::get_input_array() {
  QString text = ui->input_line->toPlainText();
  size_t size = text.size();
  char *input_array = nullptr;
  if (size > 0) {
    input_array = (char *)calloc(size + 1, sizeof(char));
    std::string convert = text.toStdString();
    for (size_t i = 0; i < size; i++) {
      input_array[i] = convert[i];
    }
    input_array[size] = '\0';
  }
  return input_array;
}

void MainWindow::free_pointer(char *pointer) {
  if (pointer != nullptr) {
    free(pointer);
    pointer = nullptr;
  }
}

double MainWindow::get_x_value() {
  double x_value = ui->x_value->text().toDouble();
  return x_value;
}

void MainWindow::set_axis_value() {
  ui->graph_show->xAxis->setRange(ui->spin_coord_x_start->value(),
                                  ui->spin_coord_x_end->value());
  ui->graph_show->yAxis->setRange(ui->spin_coord_x_start->value(),
                                  ui->spin_coord_x_end->value());
}

void MainWindow::check_axis_value() {
  if (ui->spin_coord_x_start->value() >= ui->spin_coord_x_end->value()) {
    set_default_spin_x_coord();
  }
  set_axis_value();
}

void MainWindow::on_input_line_textChanged() {
  QString text = ui->input_line->toPlainText();
  size_t size = text.size();
  if (size > 0) {
    QString check_string = "./*+-^()";
    if (text.at(size - 1).isUpper()) {
      text = text.toLower();
      ui->input_line->setText(text);
    } else if (size > SIZE_BUFFER ||
               ((check_string.indexOf(text.at(size - 1)) == -1 &&
                 !(text.at(size - 1).isLetterOrNumber())))) {
      text.remove(size - 1, 1);
      ui->input_line->setText(text);
    }
    QTextCursor cursor = ui->input_line->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->input_line->setTextCursor(cursor);
  }
}

void MainWindow::on_button_equal_clicked() {
  ui->output_line->clear();
  char *input_array = get_input_array();
  int output = input_array_validation(input_array);
  if (output == OK) {
    int amount_tokens = 0;
    token *reverse_polish_notation_array =
        reverse_polish_notation(input_array, &amount_tokens);
    if (reverse_polish_notation_array != nullptr) {
      double x_value = get_x_value();
      double result = 0.0;
      output = calculation(reverse_polish_notation_array, amount_tokens,
                           x_value, &result);
      if (output == OK) {
        set_result(result);
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

void MainWindow::set_result(const double result) {
  if (check_result(result) == OK) {
    char result_str[SIZE_BUFFER] = {'\0'};
    setlocale(LC_NUMERIC, "C");
    sprintf(result_str, "%.10g", result);
    ui->output_line->setText(result_str);
  } else {
    ui->output_line->setText(INVALID_DATA_LINE);
  }
}

int MainWindow::check_result(const double result) {
  return isinf(result) || isnan(result);
}
