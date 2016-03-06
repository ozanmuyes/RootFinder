#include "common.h"

te_expr *func = NULL;
double var_x = 0;

int get_input(char *output, unsigned int len) {
  if (output == NULL) {
    return -1;
  }

  if (len < 2) {
    return -2;
  }

  char *buffer = (char *)malloc(sizeof(char) * len);
  if (buffer == NULL) {
    return -1;
  }

  int idx = 0;

  if (fgets(buffer, len, stdin) != NULL) {
    while (*(buffer + idx) != '\n' && *(buffer + idx) != '\0' && idx < len) {
      *(output + idx) = *(buffer + idx);

      idx += 1;
    }

    *(output + idx) = '\0';

#ifdef VERBOSE_MODE
    if (len > idx) {
      fputs("WARN: Memory leak on get_input().", stderr);
    }
#endif // VERBOSE_MODE

    free(buffer);
    fflush(stdin);

    return idx + 1;
  }

  free(buffer);
  fflush(stdin);

  return 0;
}

int get_input_wo_space(char *output, unsigned int len) {
  if (output == NULL) {
    return -1;
  }

  if (len < 2) {
    return -2;
  }

  char *buffer = (char *)malloc(sizeof(char) * len);
  if (buffer == NULL) {
    return -1;
  }

  int buffer_idx = 0,
      output_idx = 0;

  if (fgets(buffer, len, stdin) != NULL) {
    while (*(buffer + buffer_idx) != '\n' && *(buffer + buffer_idx) != '\0' && buffer_idx < len) {
      if (*(buffer + buffer_idx) == ' ') {
        buffer_idx += 1;

        continue;
      }

      *(output + output_idx) = *(buffer + buffer_idx);

      buffer_idx += 1;
      output_idx += 1;
    }

    *(output + output_idx) = '\0';

#ifdef VERBOSE_MODE
    if (len > output_idx) {
      fputs("WARN: Memory leak on get_input().", stderr);
    }
#endif // VERBOSE_MODE

    free(buffer);
    fflush(stdin);

    return output_idx + 1;
  }

  free(buffer);
  fflush(stdin);

  return 0;
}

int get_input_int() {
  char *input = malloc(sizeof(char) * 11);
  get_input(input, 11);

  int result = 0;
  sscanf(input, "%d", &result);

  return result;
}

double get_input_double() {
  char *input = malloc(sizeof(char) * 20);
  get_input(input, 20);

  double result = 0;
  sscanf(input, "%lf", &result);

  return result;
}

int compile_function(const char *expression) {
  // fonksiyona ait her þeyi (deðiþkenleri fonksiyon çaðrýsý) tek bir yerde tutmak için bu proxy fonksiyonlarýný tamamla
  // te_variable deðiþkenini burada variadic arguments'e göre tanýmla ve sonunda
  // te_expr* döndürmek için te_compile() fonksiyonunu çaðýr

  // deðiþken sayýsýný bir yerde tut, f() fonksiyonu çaðrýlýrken iþe yarayabilir

  int compilation_error = 0;

  te_variable vars[] = {
    {"x", &var_x}
  };

  func = te_compile(expression, vars, 1, &compilation_error);

  if (func == NULL) {
    return -compilation_error;
  }

  return 0;
}

/**
 * Calls the compiled function with 1 parameter.
 */
double f(double variable_x) {
  var_x = variable_x;

  return te_eval(func);
}
