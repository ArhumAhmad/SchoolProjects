#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Sep 30 23:30:23 2020

@author: anthonybonner
"""
import numpy as np
import matplotlib.pyplot as plt


# In the functions below,
# X = input data
# T = data labels
# w = weight vector for decision boundary
# b = bias term for decision boundary
# elevation and azimuth are angles describing the 3D viewing direction


def boundary_mesh(X,w,w0):
    # decision boundary
    X = X.T
    xmin = np.min(X[0])
    xmax = np.max(X[0])
    zmin = np.min(X[2])
    zmax = np.max(X[2])
    x = np.linspace(xmin,xmax,2)
    z = np.linspace(zmin,zmax,2)
    xx,zz = np.meshgrid(x,z)
    yy = -(xx*w[0] + zz*w[2] + w0)/w[1]
    return xx,yy,zz


def plot_data(X,T,elevation=30,azimuth=30):
    colors = np.array(['r','b'])    # red for class 0 , blue for class 1
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    colors = np.array(['r','b'])    # red for class 0 , blue for class 1
    X = X.T
    ax.scatter(X[0],X[1],X[2],color=colors[T],s=1)
    ax.view_init(elevation,azimuth)
    plt.draw()
    return ax,fig
    

def plot_db(X,T,w,w0,elevation=30,azimuth=30):
    xx,yy,zz, = boundary_mesh(X,w,w0)
    ax,fig = plot_data(X,T,elevation,azimuth)
    ax.plot_surface(xx,yy,zz,alpha=0.5,color='green')
    return ax,fig


def plot_db3(X,T,w,w0):
    _,fig1 = plot_db(X,T,w,w0,30,0)
    _,fig2 = plot_db(X,T,w,w0,30,45)
    _,fig3 = plot_db(X,T,w,w0,30,175)
    return fig1,fig2,fig3
    

def movie_data(X,T):
    ax,fig = plot_data(X,T,30,-20)
    plt.pause(1)
    for angle in range(-20,200):
        ax.view_init(30, angle)
        plt.draw()
        plt.pause(0.0001)
    return ax
        

def movie_db(X,T,w,w0):
    xx,yy,zz,= boundary_mesh(X,w,w0)
    ax,fig = plot_data(X,T,30,-20)
    ax.plot_surface(xx,yy,zz,alpha=0.3,color='green')
    plt.pause(1)
    for angle in range(-20,200):
        ax.view_init(30, angle)
        plt.draw()
        plt.pause(0.0001)
    return ax
    

