#include <iostream>
#include <iomanip>
#include <string>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>

// Función para centrar texto en un ancho específico
static std::string centrarTexto(const std::string& texto, int ancho) {
    int espaciosIzq = (ancho - texto.size()) / 2;
    int espaciosDer = ancho - texto.size() - espaciosIzq;
    return std::string(espaciosIzq, ' ') + texto + std::string(espaciosDer, ' ');
}

int main() {
    // Variables para conexión y manejo de datos
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLHSTMT hStmt;
    SQLRETURN ret;

    // Variables para almacenar datos de cada columna
    int numEmpleado = 0, directivo = 0;
    SQLCHAR nombre[50] = "", apellidoP[50] = "", apellidoM[50] = "", fechaNac[50] = "";
    SQLCHAR rfc[20] = "", centroT[50] = "", puestoT[50] = "", descripcionP[200] = "";

    // Inicialización del entorno
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    // Conexión a la base de datos
    ret = SQLConnect(hDbc,
        (SQLWCHAR*)L"localhost", SQL_NTS,
        (SQLWCHAR*)L"Usuario", SQL_NTS,
        (SQLWCHAR*)L"Contraseña", SQL_NTS);



    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        std::cout << "Conexion exitosa a la base de datos.\n";


       


        // Preparar y ejecutar la consulta SQL
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        SQLExecDirect(hStmt, (SQLWCHAR*)L"SELECT * FROM empleados", SQL_NTS);

    
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            // Imprimir encabezados de las columnas
            std::cout << std::setfill('-') << std::setw(132) << "-" << std::endl;
            std::cout << std::setfill(' ')
                << "| " << std::setw(12) << std::left << "No.Empleado"
                << "| " << std::setw(40) << std::left << "Nombre y Apellidos"
                << "| " << std::setw(12) << std::left << "Fecha"
                << "| " << std::setw(15) << std::left << "RFC"
                << "| " << std::setw(20) << std::left << "Centro"
                << "| " << std::setw(25) << std::left << "Puesto"
                << "| " << std::setw(10) << std::left << "Directivo" << "|\n";
            std::cout << std::setfill('-') << std::setw(132) << "-" << std::endl;
            std::cout << std::setfill(' ');
        }

    
        // Obtener los resultados fila por fila
        while (SQLFetch(hStmt) == SQL_SUCCESS) {
            SQLGetData(hStmt, 1, SQL_C_LONG, &numEmpleado, 0, NULL);
            SQLGetData(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);
            SQLGetData(hStmt, 3, SQL_C_CHAR, apellidoP, sizeof(apellidoP), NULL);
            SQLGetData(hStmt, 4, SQL_C_CHAR, apellidoM, sizeof(apellidoM), NULL);
            SQLGetData(hStmt, 5, SQL_C_CHAR, fechaNac, sizeof(fechaNac), NULL);
            SQLGetData(hStmt, 6, SQL_C_CHAR, rfc, sizeof(rfc), NULL);
            SQLGetData(hStmt, 7, SQL_C_CHAR, centroT, sizeof(centroT), NULL);
            SQLGetData(hStmt, 8, SQL_C_CHAR, puestoT, sizeof(puestoT), NULL);
            SQLGetData(hStmt, 9, SQL_C_CHAR, descripcionP, sizeof(descripcionP), NULL);
            SQLGetData(hStmt, 10, SQL_C_LONG, &directivo, 0, NULL);

            nombre[sizeof(nombre) - 1] = '\0';
            apellidoP[sizeof(apellidoP) - 1] = '\0';
            apellidoM[sizeof(apellidoM) - 1] = '\0';

            std::string nombreCompleto = std::string(reinterpret_cast<char*>(nombre)) + " " +
                std::string(reinterpret_cast<char*>(apellidoP)) + " " +
                std::string(reinterpret_cast<char*>(apellidoM));
            std::string esDirectivo = (directivo == 1) ? "Sí" : "No";

            // Mostrar la información
            std::cout << "| " << std::setw(12) << centrarTexto(std::to_string(numEmpleado), 12)
                << "| " << std::setw(40) << std::left << nombreCompleto
                << "| " << std::setw(12) << std::left << fechaNac
                << "| " << std::setw(15) << std::left << rfc
                << "| " << std::setw(20) << std::left << centroT
                << "| " << std::setw(25) << std::left << puestoT
                << "| " << std::setw(10) << std::left << esDirectivo << "|\n";
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
    else {
        std::cerr << "Error al conectar con la base de datos.\n";
    }

    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}
