/*
Copyright 2007-2010 WebDriver committers
Copyright 2007-2010 Google Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

package org.openqa.selenium.remote.server.handler.html5;

import org.openqa.selenium.html5.LocationContext;
import org.openqa.selenium.remote.Response;
import org.openqa.selenium.remote.server.DriverSessions;
import org.openqa.selenium.remote.server.handler.WebDriverHandler;
import org.openqa.selenium.remote.server.rest.ResultType;

public class GetLocationContext extends WebDriverHandler {
  private volatile Response response;
  
  public GetLocationContext(DriverSessions sessions) {
    super(sessions);  
  }
  
  public Response getResponse() {
    return response;
  }
  
  public ResultType call() throws Exception {
    response = newResponse();
    response.setValue(((LocationContext) unwrap(getDriver())).location());
    return ResultType.SUCCESS;
  }
  
  @Override
  public String toString() {
    return "[get location context]";
  }
}
