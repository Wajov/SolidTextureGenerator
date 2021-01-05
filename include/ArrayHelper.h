#ifndef ARRAY_HELPER_H
#define ARRAY_HELPER_H

#include <bits/stdc++.h>

template<typename T> T *createArray1D(int n);
template<typename T> T **createArray2D(int n, int m);
template<typename T> T ***createArray3D(int n, int m, int l);
template<typename T> void deleteArray1D(T *data);
template<typename T> void deleteArray2D(T **data, int n);
template<typename T> void deleteArray3D(T ***data, int n, int m);

template<typename T> T *createArray1D(int n) {
    T *ans = new T[n];
    for (int i = 0; i < n; i++)
        ans[i] = 0;

    return ans;
}

template<typename T> T **createArray2D(int n, int m) {
    T **ans = new T *[n];
    for (int i = 0; i < n; i++) {
        ans[i] = new T[m];
        for (int j = 0; j < m; j++)
            ans[i][j] = 0;
    }

    return ans;
}

template<typename T> T ***createArray3D(int n, int m, int l) {
    T ***ans = new T **[n];
    for (int i = 0; i < n; i++) {
        ans[i] = new T *[m];
        for (int j = 0; j < m; j++) {
            ans[i][j] = new T[l];
            for (int k = 0; k < l; k++)
                ans[i][j][k] = 0;
        }
    }

    return ans;
}

template<typename T> void deleteArray1D(T *data) {
    delete []data;
}

template<typename T> void deleteArray2D(T **data, int n) {
    for (int i = 0; i < n; i++)
        delete []data[i];
    delete []data;
}

template<typename T> void deleteArray3D(T ***data, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            delete []data[i][j];
        delete []data[i];
    }
    delete []data;
}

#endif