
                            fprintf(f_poincare, "%f,\t", valores_iniciales[i]);
                        }
                        double salida_derivada[N];
                        f_derivadas(valores_iniciales, salida_derivada, t);
                        fprintf(f_poincare, "%f,\t", salida_derivada[0]);
                        fprintf(f_poincare, "%f,\t", salida_derivada[1]);
                        fprintf(f_poincare, "\n");