package com.example.hyxja.football;

import android.app.DownloadManager;
import android.content.Context;
import android.net.Uri;
import android.os.Environment;
import android.util.Log;

import org.eclipse.egit.github.core.IRepositoryIdProvider;
import org.eclipse.egit.github.core.Repository;
import org.eclipse.egit.github.core.RepositoryId;
import org.eclipse.egit.github.core.client.GitHubClient;
import org.eclipse.egit.github.core.client.GitHubRequest;
import org.eclipse.egit.github.core.service.RepositoryService;
import org.kohsuke.github.GHUser;
import org.kohsuke.github.GitHub;
import org.kohsuke.github.GHAuthorization;
import org.kohsuke.github.HttpConnector;

import java.io.File;
import java.io.IOException;

/**
 * Created by hyxja on 2016/9/28.
 */
public class FetchData {

    public FetchData(Context context) {
        String serviceString = context.DOWNLOAD_SERVICE;
        DownloadManager downloadManager;
        downloadManager = (DownloadManager)context.getSystemService(serviceString);
        Uri uri = Uri.parse("https://github.com/publicsjtu/football");
        DownloadManager.Request request = new DownloadManager.Request(uri);
        request.setDestinationInExternalPublicDir(Environment.getExternalStorageDirectory().getPath(),
                "test1.db");
        long id = downloadManager.enqueue(request);
    }
}
